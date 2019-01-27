#pragma once

#include <Windows.h>
#include <string>
#include <vector>

namespace alib
{
	typedef std::vector<unsigned char> KeyEvents;

	class KeyboardEvents
	{
	public:

		void run()
		{
			for (int ix = 0; ix < 256; ix++)
			{
				m_oldEvents[ix] = m_newEvents[ix];
				m_newEvents[ix] = !!GetAsyncKeyState(ix);
				if (m_newEvents[ix])
					m_heldTime[ix]++;
				else
					m_heldTime[ix] = 0;
			}
		}

		bool isKeyDown(unsigned char key)
		{
			return m_newEvents[key];
		}
		bool isKeyTyped(unsigned char key)
		{
			return isKeyPressed(key) || m_heldTime[key] > m_heldTimeThreshold && !(m_heldTime[key] % m_heldTimeRollover);
		}

		bool isKeyUp(unsigned char key)
		{
			return !m_newEvents[key];
		}

		bool isKeyReleased(unsigned char key)
		{
			return !m_newEvents[key] && m_oldEvents[key];
		}

		bool isKeyPressed(unsigned char key)
		{
			return m_newEvents[key] && !m_oldEvents[key];
		}

		char getChar()
		{
			for (int i = 0; i < 48; ++i)
				if (isKeyPressed(m_keyList[i][0]))
					if (isKeyDown(VK_SHIFT))
						return m_keyList[i][2];
					else
						return m_keyList[i][1];
			return -1;
		}
		int getTyped()
		{
			for (int i = 0; i < 48; ++i)
				if (isKeyTyped(m_keyList[i][0]))
					if (isKeyDown(VK_SHIFT))
						return m_keyList[i][2];
					else
						return m_keyList[i][1];
			return -1;
		}

		int getKeyPressed()
		{
			for (int i = 0; i < 256; ++i)
				if (isKeyPressed(i))
					return i;
			return -1;
		}
		int getKeyDown()
		{
			for (int i = 0; i < 256; ++i)
				if (isKeyDown(i))
					return i;
			return -1;
		}		
		int getKeyReleased()
		{
			for (int i = 0; i < 256; ++i)
				if (isKeyReleased(i))
					return i;
			return -1;
		}
		KeyEvents getAllPressed()
		{
			KeyEvents v;
			for (int i = 0; i < 256; ++i)
				if (isKeyPressed(i))
					v.push_back(i);
			return v;
		}
		KeyEvents getAllReleased()
		{
			KeyEvents v;
			for (int i = 0; i < 256; ++i)
				if (isKeyReleased(i))
					v.push_back(i);
			return v;
		}

		KeyboardEvents() {}
	private:
		unsigned char m_oldEvents[256] = { 0 };
		unsigned char m_newEvents[256] = { 0 };
		int m_heldTime[256] = { 0 };
		int m_heldTimeThreshold = 25;
		int m_heldTimeRollover = 3;

		// KEY LIST //
		 char			m_keyList[54][3] =
		{
			{ (char)0x41, 'a', 'A' },
			{ (char)0x42, 'b', 'B' },
			{ (char)0x43, 'c', 'C' },
			{ (char)0x44, 'd', 'D' },
			{ (char)0x45, 'e', 'E' },
			{ (char)0x46, 'f', 'F' },
			{ (char)0x47, 'g', 'G' },
			{ (char)0x48, 'h', 'H' },
			{ (char)0x49, 'i', 'I' },
			{ (char)0x4A, 'j', 'J' },
			{ (char)0x4B, 'k', 'K' },
			{  (char) 0x4C, 'l', 'L' },
			{  (char) 0x4D, 'm', 'M' },
			{  (char) 0x4E, 'n', 'N' },
			{  (char) 0x4F, 'o', 'O' },
			{  (char) 0x50, 'p', 'P' },
			{  (char) 0x51, 'q', 'Q' },
			{  (char) 0x52, 'r', 'R' },
			{  (char) 0x53, 's', 'S' },
			{  (char) 0x54, 't', 'T' },
			{  (char) 0x55, 'u', 'U' },
			{  (char) 0x56, 'v', 'V' },
			{  (char) 0x57, 'w', 'W' },
			{  (char) 0x58, 'x', 'X' },
			{  (char) 0x59, 'y', 'Y' },
			{  (char) 0x5A, 'z', 'Z' },
			{  (char) 0x30, '0', ')' },
			{  (char) 0x31, '1', '!' },
			{  (char) 0x32, '2', '"' },
			{  (char) 0x33, '3', '£' },
			{  (char) 0x34, '4', '$' },
			{  (char) 0x35, '5', '%' },
			{  (char) 0x36, '6', '^' },
			{  (char) 0x37, '7', '&' },
			{  (char) 0x38, '8', '*' },
			{  (char) 0x39, '9', '(' },
			{  (char) 0xDC, '\\', '|' },
			{  (char) 0xBC, ',', '<' },
			{  (char) 0xBE, '.', '>' },
			{  (char) 0xBF, '/', '?' },
			{  (char) 0xBA, ';', ':' },
			{  (char) 0xC0, '\'', '@' },
			{  (char) 0xDE, '#', '~' },
			{  (char) 0xBD, '-', '_' },
			{  (char) 0xBB, '=', '+' },
			{  (char) 0xDB, '[', '{' },
			{  (char) 0xDD, ']', '}' },
			{  (char) 0x20, ' ', ' ' },
			{  (char) VK_LBUTTON, '\0', '\0' },
			{  (char) VK_RBUTTON, '\0', '\0' },
			{  (char) VK_LEFT, '\0', '\0' },
			{  (char) VK_RIGHT, '\0', '\0' },
			{  (char) VK_UP, '\0', '\0' },
			{  (char) VK_DOWN, '\0', '\0' }
		};
	};


}