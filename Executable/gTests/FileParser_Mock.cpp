# include "stdafx.h"
# include "FileParser_Mock.h"
# include "..\..\Model\MidiParserLib\FileCounter.h"
# include "..\..\Model\MidiParserLib\MidiStruct.h"

using namespace std;
using namespace Model;
using namespace MidiParser;
using MidiStruct::Bytes;

FileParser_Mock::FileParser_Mock(const char* fileName) :
	IFileParser(),
	inputFile_(fileName, std::ifstream::in),
	bytesRemained_(make_shared<FileCounter>())
{}
FileParser_Mock::~FileParser_Mock() {}


int FileParser_Mock::GetBytesRemained_impl() const
{
	return bytesRemained_->Get();
}
void FileParser_Mock::SetBytesRemained_impl(const int value) const
{
	bytesRemained_->Set(value);
}


# define NOT_IMPLEMENTED BORIS_ASSERT("VIRTUAL FUNCTION " __FUNCTION__ " HAS NOT BEEN OVERRIDEN");

int FileParser_Mock::PeekByte_impl()
{
	NOT_IMPLEMENTED;
	return NULL;
}
char FileParser_Mock::ReadByte_impl()
{
	string str;
	getline(inputFile_, str);
	bytesRemained_->Reduce(1);

	auto result(NULL);
	istringstream(str) >> result;
	return static_cast<char>(result);
}
void FileParser_Mock::ReadData_impl(char*, std::streamsize)
{
	NOT_IMPLEMENTED;
}
void FileParser_Mock::SkipData_impl(std::streamoff offset)
{
	assert("CURRENT MOCK OBJECT CAN HANDLE ONLY FORWARD MOVES" && offset >= 0);
	for (auto i(NULL); i < offset; ++i) ReadByte();
}

unsigned FileParser_Mock::ReadInverse_impl(unsigned, bool)
{
	NOT_IMPLEMENTED;
	return NULL;
}
unsigned FileParser_Mock::ReadVarLenFormat_impl()
{
	auto result(NULL),
		totalBytes(NULL);
	auto anotherByte('\0');

	for (; (anotherByte = ReadByte()) < 0; result -= anotherByte)	// ends with the positive byte
	{
		if (++totalBytes >= Bytes::varLengthSize)
			throw length_error("UNEXPECTED VARIABLE LENGTH > FOUR BYTES");
	}
	return static_cast<unsigned>(result + anotherByte);
}


# define NOT_SPECIALIZED BORIS_ASSERT("TEMPLATE " __FUNCTION__ " HAS NOT BEEN SPECIALIZED");

uint32_t MidiParser::ReadWord(std::shared_ptr<FileParser_Mock>)	// Word = 4 bytes!!!
{
	NOT_SPECIALIZED;
	return NULL;
}
uint16_t MidiParser::ReadDWord(std::shared_ptr<FileParser_Mock>)	// DWord = 2 bytes!!!
{
	NOT_SPECIALIZED;
	return NULL;
}