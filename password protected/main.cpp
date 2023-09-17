#pragma warning (disable : 26451) // 4byte to 8byte

#include "PatchUtils.h"
#include "encryption.h"
#include "strobf.h"

/*
e8 00 00 00 00 b9 f5 ff
x????xxx
*/


// ayo u confused fam? heres da explanation, a sig is an array o' bytes dat you can search for in order to find de addrezz
// and if da mask says ? then the byte can be anythin' yo, 'cus it change a lot wit' the code changin' but the sig dat I have rn is allg wit' dat sorta stuff yk fam
// and if da mask says x it's all g fam n checks it normally to see if it lines up
// n da offset is just the offset to the real addrezz instead of the sig addrezz fr fam
static char patchfilesig[] = 
	{ 0xe8, 0x00, 0x00, 0x00, 0x00, 0x83, 0x3d};

static char patchfilemask[] =
	{ "x????xx" };

const int sigoffset = 0;

char pass[8 + 1]; // + 1 fo' dat null-terminator fam
int setupflag = 0; // set to 1 if patchfile is called (if 1, this is the setup. If 0, this is a password-protected .exe)


// setconsoletextattr(10 = green, 4 = red, 1 = blue, 0 = black)


void EncryptedFunc()
{
	// start marker
	for (size_t i = 0x777; i < 0x777; i++) { }

	HANDLE hCout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hCout, 10);
	std::cout << AY_OBFUSCATE("If you can see this, you have either decrypted the function, or just decrypted the string.\n");
	std::cout << AY_OBFUSCATE("The goal is to run this function decrypted, not decrypt these strings.\n");
	SetConsoleTextAttribute(hCout, 4);
	std::cout << AY_OBFUSCATE("Good luck.\n\n\n");
	
	// blue
	SetConsoleTextAttribute(hCout, ((4182376 ^ 4271386) * 423768) % 3);
	std::cout << AY_OBFUSCATE("So i've been thinking, is this uncrackable? Because like there are a lot of different keys that result in non-erroring opcodes right?\n So could you even brute force this? Idk.\n");
	std::cout << AY_OBFUSCATE("On a more serious note, if you have actually decrypted this function then take a screenshot of it and post it in comments. Also tell me how you did it in good detail.\n\n");
	
	// grey
	SetConsoleTextAttribute(hCout, ((6 ^ 8) * 4444) % 12);
	std::cout << AY_OBFUSCATE("I'm not really sure what else to put in this function tbh so im just doing this\n");
	std::cout << AY_OBFUSCATE("This took me a while to make... Tell me in the comments if this is actually uncrackable tho.\n");
	std::cout << AY_OBFUSCATE("If you want me to release the horrid src code i can.\n");

	// end marker
	for (size_t i = 0x1000; i < 0x1000; i++) { }
}


void GetPass()
{
	HANDLE hCout = GetStdHandle(STD_OUTPUT_HANDLE);
	int ishiddenpass = 0;

	SetConsoleTextAttribute(hCout, 10);
	std::cout << "[+] ayo wanna hide yo pass while enterin' dat? (1 for yes 0 for no)\n> ";
	SetConsoleTextAttribute(hCout, 4);
	std::cin >> ishiddenpass;
	SetConsoleTextAttribute(hCout, 10);
	std::cout << "[+] fr whass da password dawg?\n> ";
	if (ishiddenpass)
	{
		SetConsoleTextAttribute(hCout, 0);
	}
	else
	{
		SetConsoleTextAttribute(hCout, 4);
	}
	std::cin >> pass;
	SetConsoleTextAttribute(hCout, 10);

	// maybe change this to encrypting for the unpatched file, then sigscan and patch the string to decrypting for the protected file. Do this to polish it up a bit
	std::cout << "\n[+] yo we be cryptographyin' rn just hol' up fam\n";

	SetConsoleTextAttribute(hCout, 7);
}


// this method NOPs the call to itself, then it encrypts the file so that it can be decrypted at runtime.
void patchfile()
{
	setupflag = 1;

	std::fstream fileStream;

	fileStream.open("C:\\_Random\\patchme.exe", std::ios::in | std::ios::out | std::ios::binary);

	size_t sigresult = SigScan(&fileStream, patchfilesig, patchfilemask, 8, 1);

	// this is the size of the call to the thing
	size_t funcsize = 5;

	std::cout << "origional: ";
	CoutBytes(&fileStream, sigresult + sigoffset, 5);

	char* nops = new char[funcsize];
	for (size_t i = 0; i < funcsize; i++)
	{
		nops[i] = '\x90';
	}

	PatchBytes(&fileStream, sigresult + sigoffset, nops, funcsize);

	delete[] nops;

	std::cout << "Patched: ";
	CoutBytes(&fileStream, sigresult + sigoffset, 5);

	std::cout << "sigscan result: 0x" << std::hex << sigresult << std::dec << '\n';
	std::cout << "\n THIS IS THE SETUP, WHATEVER PASSWORD YOU PUT IN YOU WILL NEED TO USE TO UNLOCK THE OUTPUTTED FILE\n";

	GetPass();

	// do some cryptographyin'
	xorpatch(&fileStream, pass);
	fileStream.close();

	return;
}


int main()
{
	patchfile();
	if (setupflag == 1) { return 1; }

	GetPass();

	
	if (xorsigmem(pass, (uintptr_t)&EncryptedFunc) == false)
	{
		std::cout << "do some more debugging bro\n";
		return 0xdeadbeef;
	}

	std::string kjfnsd = "marker";
	EncryptedFunc();

	Sleep(0xFFFFFFFF);
	return 0b00000000;
}