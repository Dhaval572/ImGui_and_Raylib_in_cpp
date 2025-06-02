// YouTube video downloader
#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include <thread>
#include <atomic>
#include <raylib.h>
#include <rlImGui.h>
#include <imgui.h>
#include "ImGuiCustomTheme.h"
using namespace std;

string statusMessage = "";
bool isError = false;
atomic<bool> isInstalling(false);
atomic<bool> isDownloading(false);
atomic<bool> shouldCancelDownload(false);
atomic<float> installProgress(0.0f);
string currentInstallPackage = "";
bool showDownloadConfirmation = false;
string pendingUrl = "";
string pendingFormat = "";
string pendingPath = "";

void setStatus(const string &message, bool error = false)
{
    statusMessage = message;
    isError = error;
}

string escapeShellArg(const string &arg)
{
    return "\"" + arg + "\"";
}

bool commandExists(const string &cmd)
{
    return system(("where " + cmd + " > nul 2>&1").c_str()) == 0;
}

void installWithWinget(const string &packageName)
{
    currentInstallPackage = packageName;
    isInstalling = true;
    installProgress = 0.0f;

    thread([packageName]()
           {
        setStatus("Installing " + packageName + " using winget...");
        
        // Simulate progress (in real app you'd parse actual progress)
        for (uint8_t i = 0; i < 10; i++) 
        {
            this_thread::sleep_for(chrono::milliseconds(300));
            installProgress = (i + 1) * 0.1f;
        }
        
        int result = system(("winget install --accept-package-agreements --accept-source-agreements " + packageName + " > nul 2>&1").c_str());
        
        isInstalling = false;
        installProgress = 0.0f;
        
        if (result != 0) 
        {
            setStatus("Failed to install " + packageName + ". Please install it manually.", true);
        } else 
        {
            setStatus(packageName + " installed successfully!");
        } })
        .detach();
}

bool ensureYtDlpInstalled()
{
    if (commandExists("yt-dlp"))
        return true;

    if (!isInstalling)
    {
        installWithWinget("yt-dlp.yt-dlp");
    }
    return false;
}

bool ensureFfmpegInstalled()
{
    if (commandExists("ffmpeg"))
        return true;

    if (!isInstalling)
    {
        installWithWinget("ffmpeg");
    }
    return false;
}

string buildDownloadCommand(const string &url, const string &format, const string &outputPath)
{
    string outputOption = " -P " + escapeShellArg(outputPath);
    if (format == "mp4")
        return "yt-dlp -f \"bestvideo[ext=mp4]+bestaudio[ext=m4a]/best[ext=mp4]/best\" --merge-output-format mp4" + outputOption + " " + url;
    else
        return "yt-dlp -x --audio-format mp3 --audio-quality 0 --embed-thumbnail" + outputOption + " " + url;
}

void downloadVideo(const string &url, const string &format, const string &path)
{
    isDownloading = true;
    shouldCancelDownload = false;
    setStatus("Preparing download...");

    thread([url, format, path]()
           {
        
               string command = buildDownloadCommand(escapeShellArg(url), format, path);

               // First check if we can start the download
               setStatus("Checking requirements...");
               if (!commandExists("yt-dlp"))
               {
                   setStatus("Error: yt-dlp not installed. Please install it first.", true);
                   isDownloading = false;
                   return;
               }

               setStatus("Starting download...");

               // Open a new terminal window to show progress
               string terminalCommand = "start cmd /k \"" + command + "\"";
               int result = system(terminalCommand.c_str());

               // Track if we got a proper completion
               bool properCompletion = false;

               // While the download is in progress
               while (result == 0 && !shouldCancelDownload)
               {
                   setStatus("Downloading...");
                   this_thread::sleep_for(chrono::milliseconds(100));

                   // Check if process is still running
                   if (system("tasklist | find \"yt-dlp.exe\" > nul") != 0)
                   {
                       properCompletion = true;
                       break;
                   }
               }

               if (shouldCancelDownload)
               {
                   // Additional cleanup
                   system("taskkill /f /im yt-dlp.exe > nul 2>&1");
                   setStatus("Download canceled by user.", true);
               }
               else if (properCompletion)
               {
                   setStatus("Downloading video please wait... ");
               }
               else
               {
                   setStatus("Something went wrong with the download", true);
               }

               isDownloading = false; })
        .detach();
}

int main()
{
    InitWindow(800, 600, "yt-dlp GUI - Raylib + ImGui");
    SetTargetFPS(60);

    rlImGuiSetup(true);
    ImCustomTheme();

    char urlBuffer[2048] = "";
    char pathBuffer[25] = "Yt_videos";
    const char *formats[] = {"mp3", "mp4"};
    int currentFormatIndex = 0;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        rlImGuiBegin();
        ImGui::Begin("YouTube Downloader");

        ImGui::Text("Enter video URL:");
        ImGui::InputTextMultiline("##url", urlBuffer, sizeof(urlBuffer), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 5), ImGuiInputTextFlags_AllowTabInput);

        ImGui::Text("Choose format:");
        ImGui::Combo("##format", &currentFormatIndex, formats, IM_ARRAYSIZE(formats));

        ImGui::Text("Download folder:");
        ImGui::InputText("##path", pathBuffer, sizeof(pathBuffer));

        bool canDownload = !isInstalling && !isDownloading && strlen(urlBuffer) > 0;
        if (!canDownload)
            ImGui::BeginDisabled();

        if (ImGui::Button("Download"))
        {
            string url = string(urlBuffer);
            string format = formats[currentFormatIndex];
            string path = string(pathBuffer);

            if (url.empty())
            {
                setStatus("Error: URL cannot be empty.", true);
            }
            else if (!commandExists("yt-dlp") && !isInstalling)
            {
                ensureYtDlpInstalled();
                setStatus("Please wait while yt-dlp is being installed...");
            }
            else
            {
                if (!commandExists("ffmpeg") && !isInstalling)
                {
                    ensureFfmpegInstalled();
                }
                // Store the download parameters and show confirmation popup
                pendingUrl = url;
                pendingFormat = format;
                pendingPath = path;
                showDownloadConfirmation = true;
            }
        }

        if (!canDownload)
            ImGui::EndDisabled();

        // Show the download confirmation popup
        if (showDownloadConfirmation)
        {
            ImGui::OpenPopup("Download Confirmation");
            if (ImGui::BeginPopupModal("Download Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Really want to download?");
                ImGui::Separator();

                if (ImGui::Button("Start", ImVec2(120, 0)))
                {
                    // Start the download
                    downloadVideo(pendingUrl, pendingFormat, pendingPath);
                    showDownloadConfirmation = false;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    // Cancel the download
                    showDownloadConfirmation = false;
                    setStatus("Download canceled by user.", true);
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
        }

        // Show installation progress if installing
        if (isInstalling)
        {
            ImGui::TextColored(ImVec4(1, 1, 0.3f, 1), "Installing %s...", currentInstallPackage.c_str());
            ImGui::ProgressBar(installProgress, ImVec2(-1.0f, 0.0f));
            ImGui::Text("Please wait, this may take a few minutes...");
        }

        // Show downloading status
        else if (isDownloading)
        {
            ImGui::TextColored(ImVec4(0.3f, 1, 0.3f, 1), "Download in progress...");
            ImGui::Text("Check the terminal window for progress details");

            if (ImGui::Button("Cancel Download"))
            {
                shouldCancelDownload = true;
                setStatus("Canceling download...", true);
            }
        }

        if (WindowShouldClose() && isDownloading)
        {
            shouldCancelDownload = true;
            setStatus("Download interrupted by window close", true);
            // Wait a moment for cleanup
            this_thread::sleep_for(chrono::milliseconds(200));
        }

        // Show status message
        if (!statusMessage.empty())
        {
            if (isError)
                ImGui::TextColored(ImVec4(1, 0.3f, 0.3f, 1), "%s", statusMessage.c_str());
            else
                ImGui::TextColored(ImVec4(0.3f, 1, 0.3f, 1), "%s", statusMessage.c_str());
        }

        ImGui::End();
        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
