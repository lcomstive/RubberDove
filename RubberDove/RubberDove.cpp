/*
	Rubber Dove - Like the Ducky, except it's not

	Copyright (C) 2017  lcomstive

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/
#ifndef _WIN32
#error "RubberDove is currently only supported on Windows!"
#endif
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <direct.h>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#define WINVER 0x0500
#define _WIN32_WINNT 0x0500
#define INPUT_FILE "./input.rds"
#define ERROR_FILE "./errors.txt"

using namespace std;

struct KeyCommand
{
	WORD key;
	WORD key2;
	WORD key3;
	unsigned int delay;

	KeyCommand()
	{
		key = NULL;
		key2 = NULL;
		key3 = NULL;
		delay = 0;
	}
};

bool rawText = false;
vector<KeyCommand> keys;

bool StartsWith(string a, string b)
{
	if (a.length() < b.length())
		return false;
	int index = 0;
	while (index < b.length())
	{
		if (a[index] != b[index])
			return false;
		index++;
	}
	return true;
}

static KeyCommand CreateKey(WORD key, unsigned int delay = 0)
{
	KeyCommand cmd;
	cmd.key = key;
	cmd.delay = delay;
	return cmd;
}

static KeyCommand CreateKeyCombo(WORD key1, WORD key2)
{
	KeyCommand cmd;
	cmd.key = key1;
	cmd.key2 = key2;
	return cmd;
}

static KeyCommand CreateKeyCombo(WORD key1, WORD key2, WORD key3)
{
	KeyCommand cmd;
	cmd.key = key1;
	cmd.key2 = key2;
	cmd.key3 = key3;
	return cmd;
}

string VirtualKeycodeToString(WORD key)
{
	unsigned int scancode = MapVirtualKey(key, MAPVK_VK_TO_VSC);

	char szName[128];
	int result = 0;
	switch (key)
	{
	case VK_LEFT:
	case VK_RIGHT:
	case VK_UP:
	case VK_DOWN:
	case VK_PRIOR:
	case VK_NEXT:
	case VK_END:
	case VK_HOME:
	case VK_INSERT:
	case VK_DELETE:
	case VK_DIVIDE:
	case VK_NUMLOCK:
		scancode |= 0x100;
	default:
		result = GetKeyNameTextA(scancode << 16, szName, 128);
		break;
	}
	if (result == 0)
		cerr << "WinAPI error!" << endl;
	return szName;
}

bool Shift(char c)
{
	if (isupper(c))
		return true;
	if (isalpha(c) || isdigit(c))
		return false;
	return (c == '!' || c == '@' || c == '#' || c == '$' ||
		c == '%' || c == '^' || c == '&' || c == '*' ||
		c == '(' || c == ')' || c == '_' || c == '+' ||
		c == '~' || c == '<' || c == '>' || c == '?' ||
		c == '{' || c == '}' || c == '|' || c == '"' || 
		c == ':');
}

WORD GetKey(string name)
{
	if (name.length() == 0)
		return NULL;
	string lower = name;
	transform(name.begin(), name.end(), lower.begin(), ::tolower);
	if (lower == "gui" || lower == "windows")
		return VK_LWIN;
	else if (lower == "shift")
		return VK_LSHIFT;
	else if (lower == "ctrl" || lower == "control")
		return VK_LCONTROL;
	else if (lower == "tab")
		return VK_TAB;
	else if (lower == "capslock")
		return VK_CAPITAL;
	else if (lower == "esc" || lower == "escape")
		return VK_ESCAPE;
	else if (lower == "alt")
		return VK_MENU;
	else if (lower == "enter")
		return VK_RETURN;
	else if (lower == "backspace")
		return VK_BACK;
	else if (lower == "delete")
		return VK_DELETE;
	else if (lower == "left")
		return VK_LEFT;
	else if (lower == "right")
		return VK_RIGHT;
	else if (lower == "up")
		return VK_UP;
	else if (lower == "down")
		return VK_DOWN;
	else if (lower == "numlock")
		return VK_NUMLOCK;
	else if (lower == "pageup")
		return VK_PRIOR;
	else if (lower == "pagedown")
		return VK_NEXT;
	else if (lower == "space")
		return VK_SPACE;
	else if (lower == "printscreen" || lower == "snapshot")
		return VK_SNAPSHOT;
	else if (lower == "insert")
		return VK_INSERT;
	else if (lower == "volume_mute")
		return VK_VOLUME_MUTE;
	else if (lower == "volume_up")
		return VK_VOLUME_UP;
	else if (lower == "volume_down")
		return VK_VOLUME_DOWN;
	else if (lower == "volume_skip")
		return VK_MEDIA_NEXT_TRACK;
	else if (lower == "volume_prev")
		return VK_MEDIA_PREV_TRACK;
	else if (lower == "volume_playpause")
		return VK_MEDIA_PLAY_PAUSE;
	else if (lower == "menu")
		return VK_MENU;
	else if (lower[0] == 'f')
	{
		if (lower.length() > 1)
		{
			switch (stoi(lower.substr(1)))
			{
			default:
			case 1: return VK_F1;
			case 2: return VK_F2;
			case 3: return VK_F3;
			case 4: return VK_F4;
			case 5: return VK_F5;
			case 6: return VK_F6;
			case 7: return VK_F7;
			case 8: return VK_F8;
			case 9: return VK_F9;
			case 10: return VK_F10;
			case 11: return VK_F11;
			case 12: return VK_F12;
			case 13: return VK_F13;
			case 14: return VK_F14;
			case 15: return VK_F15;
			// Windows supports up to F24 but I've never seen anybody with that many
			}
		}
	}
	return NULL;
}

bool ProcessLine(vector<string>* split)
{
	WORD temp = NULL;
	if (split->size() == 0 || (temp = GetKey(split->at(0))) == NULL || rawText)
		return false;
	WORD key = temp;
	WORD key2 = NULL, key3 = NULL;

	for (int i = 1; i < split->size(); i++)
	{
		if ((temp = GetKey(split->at(i))) == NULL)
		{
			for (int j = 0; j < split->at(i).size(); j++)
			{
				if (key2 == NULL)
					key2 = VkKeyScan(split->at(i)[j]);
				else if (key3 == NULL)
					key3 = VkKeyScan(split->at(i)[j]);
				else
					cerr << "Maximum keys in a combination is currently 3! '" << split->at(i) << "' (0)" << endl;
			}
		}
		else
		{
			if (key2 == NULL)
				key2 = temp;
			else if (key3 == NULL)
				key3 = temp;
			else
				cerr << "Maximum keys in a combination is currently 3! '" << split->at(i) << "' (1)" << endl;
			temp = NULL;
		}
	}
	if(split->size() == 0)
		keys.push_back(CreateKey(key));
	if (key3 != NULL)
		keys.push_back(CreateKeyCombo(key, key2, key3));
	else if (key2 != NULL)
		keys.push_back(CreateKeyCombo(key, key2));
	else
		keys.push_back(CreateKey(key));
	return true;
}

/**  Reference to Sayyed Mostafa Hashemi - Change Master Volume in Visual C++ - Written 29 July 2011
		https://www.codeproject.com/Tips/233484/Change-Master-Volume-in-Visual-Cplusplus 
**/
void SetVolume(unsigned int volumePercent)
{
	CoInitialize(NULL);
	IMMDeviceEnumerator* deviceEnumerator = NULL;
	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);
	
	IMMDevice* defaultDevice = NULL;
	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	deviceEnumerator->Release();
	deviceEnumerator = NULL;

	IAudioEndpointVolume* endpointVolume = NULL;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&endpointVolume);
	defaultDevice->Release();
	defaultDevice = NULL;

	hr = endpointVolume->SetMasterVolumeLevelScalar((float)volumePercent / 100.0f, NULL);
	endpointVolume->Release();
	CoUninitialize();
}

unsigned int default_delay = 20;
int main()
{
	ofstream errorStream((const char*)ERROR_FILE, ios::out | ios::app);
	std::cerr.rdbuf(errorStream.rdbuf());
	std::cout.rdbuf(errorStream.rdbuf());

	ShowWindow(GetConsoleWindow(), SW_HIDE);
	ifstream file;
	file.open((const char*)INPUT_FILE);
	if (!file.is_open())
	{
		ofstream oFile((const char*)INPUT_FILE, ios::out);
		oFile << "default_delay 50" << endl;
		oFile << "WINDOWS r" << endl;
		oFile << "notepad" << endl;
		oFile << "ENTER" << endl;
		oFile << "DELAY 100" << endl;
		oFile << "Hello, world!" << endl;
		oFile.flush();
		oFile.close();
		file.open((const char*)INPUT_FILE, ios::out);
	}

	string line, lower;
	vector<string> split;
	while (getline(file, line))
	{
		if (line.length() == 0)
			continue;
		rawText = false;
		lower = line;
		vector<string> split{ istream_iterator<string> { istringstream(line) }, { istream_iterator<string>{}} };
		transform(line.begin(), line.end(), lower.begin(), ::tolower);
		if (StartsWith(lower, "rem"))
			continue;
		if (StartsWith(lower, "string"))
		{
			rawText = true;
			line = line.substr(7);
		}
		if (StartsWith(lower, "default_delay"))
		{
			if (split.size() > 1)
			{
				try
				{
					default_delay = stoi(split[1]);
				}
				catch (exception e)
				{
					cerr << "Error setting default delay! " << e.what() << "\nPress any key to continue..." << endl;
				}
			}
			continue;
		}
		if (StartsWith(lower, "setvolume"))
		{
			if (split.size() > 1)
			{
				try
				{
					unsigned int volume = stoi(split[1]);
					SetVolume(volume);
				}
				catch (exception e)
				{
					cerr << "Error setting volume! " << e.what() << "\nPress any key to continue..." << endl;
				}
			}
			continue;
		}
		if (ProcessLine(&split))
			continue;
		if (StartsWith(lower, "ctrl-shift"))
		{
			keys.push_back(CreateKeyCombo(VK_LCONTROL, VK_LSHIFT));
			lower = lower.substr(11);
			line = line.substr(11);
			split.erase(split.begin()); // erase 'ctrl-shift'
			ProcessLine(&split);
			continue;
		}
		if (line.length() >= 5 && StartsWith(lower, "delay"))
		{
			if (line.length() == 5)
				continue;
			try
			{
				line = line.substr(6);
				lower = lower.substr(6);
				keys.push_back(CreateKey(NULL, stoi(line)));
			}
			catch (exception) {}
			continue;
		}
		for (unsigned int i = 0; i < line.length(); i++)
		{
			if (StartsWith(lower.substr(i), "_dir_"))
			{
				char dir[MAX_PATH];
				_getcwd(dir, sizeof(dir));
				lower = lower.substr(0, i) + string((char*)dir) + lower.substr(i + 5);
				line = line.substr(0, i) + string((char*)dir) + line.substr(i + 5);
			}
			if (Shift(line[i]))
				keys.push_back(CreateKeyCombo(VK_LSHIFT, VkKeyScan(line[i])));
			else
				keys.push_back(CreateKey(VkKeyScan(line[i])));
		}
	}
	
	file.close();
	
	INPUT ip;
	for (unsigned int i = 0; i < keys.size(); i++)
	{
		if (keys[i].delay > 0)
		{
			Sleep(keys[i].delay);
			continue;
		}

		ip.type = INPUT_KEYBOARD;
		ip.ki.wScan = 0;
		ip.ki.time = 0;
		ip.ki.dwExtraInfo = 0;
		ip.ki.dwFlags = 0;
		if (keys[i].key != NULL)
		{
			ip.ki.wVk = keys[i].key;
			SendInput(1, &ip, sizeof(INPUT));
			if (keys[i].key2 != NULL)
			{
				ip.ki.wVk = keys[i].key2;
				SendInput(1, &ip, sizeof(INPUT));
				if (keys[i].key3 != NULL)
				{
					ip.ki.wVk = keys[i].key3;
					SendInput(1, &ip, sizeof(INPUT));
				}
			}
		}
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		if (keys[i].key != NULL)
		{
			ip.ki.wVk = keys[i].key;
			SendInput(1, &ip, sizeof(INPUT));
			if (keys[i].key2 != NULL)
			{
				ip.ki.wVk = keys[i].key2;
				SendInput(1, &ip, sizeof(INPUT));
				if (keys[i].key3 != NULL)
				{
					ip.ki.wVk = keys[i].key3;
					SendInput(1, &ip, sizeof(INPUT));
				}
			}
		}
		Sleep(default_delay);
	}

	return 0;
}

