#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

#define MAGIC_START 0x128
#define MAGIC_LENGTH 257
#define HEADER_LENGTH (MAGIC_START + MAGIC_LENGTH)
#define MAGIC_BYTE 0x7B
#define DATA_START 0x22C

void usage(const char* argv[])
{
    cout << "Usage: " << argv[0] << " e <encrypted_file> <decrypted_file> <output_file>" << endl;
    cout << "       " << argv[0] << " d <encrypted_file> <decrpyted_file>" << endl;
}

int main(int argc, const char* argv[])
{
    bool decrypt = false;
    if (argc < 4)
    {
        usage(argv);
        return -1;
    }

    decrypt = strcmp(argv[1], "d") == 0;
    cout << ((decrypt) ? "Decryption Mode." : "Re-encryption mode.") << endl;

    if (!decrypt)
    {
        if (argc < 5)
        {
            usage(argv);
            return -1;
        }
    }

    const char* encrypted_file_name = argv[2];
    const char* decrypted_file_name = argv[3];
    const char* out_name = argv[(decrypt)? 3 : 4];

    ifstream encrypted(encrypted_file_name, ifstream::binary);
    ifstream decrypted;
    ofstream out(out_name, ofstream::binary);

    size_t length = 0;
    if (decrypt)
    {
        encrypted.seekg(0, ios_base::end);
        length = encrypted.tellg() - (size_t)DATA_START;

        if (length < DATA_START)
        {
            cout << "ERROR: Encrypted file too short, only " << length << " bytes." << endl;
            return -1;
        }
        else
        {
            cout << "Reading " << length << " bytes from " << encrypted_file_name << "." << endl;
        }
    }
    else
    {
        // re-encrpyting
        decrypted.open(decrypted_file_name, ifstream::binary);
        decrypted.seekg(0, ios_base::end);
        length = decrypted.tellg();
        cout << "Reading " << length << " bytes from " << decrypted_file_name << "." << endl;
    }

    if (!decrypt)
    {
        // Seek to start to copy the header.
        cout << "Writing header..." << endl;
        encrypted.seekg(0, ios_base::beg);
        char header[HEADER_LENGTH];
        encrypted.read(header, HEADER_LENGTH);
        out.write(header, HEADER_LENGTH);
        for (size_t i = HEADER_LENGTH; i < DATA_START; ++i) {
            out.put('\0');
        }
        cout << "Inserted " << DATA_START - HEADER_LENGTH << " padding bytes." << endl;
        cout << "Done" << endl;
    }

    // Seek to magic to copy the magic.
    cout << "Reading magic..." << endl;
    char magic[MAGIC_LENGTH];
    encrypted.seekg(MAGIC_START, ios_base::beg);
    encrypted.read(magic, MAGIC_LENGTH);
    cout << "Done" << endl;

    // Read remainder of file (all at once... hopefully it's small).
    cout << "Copying remaining data into memory..." << endl;
    char *data = new char[length];
    if (decrypt)
    {
        encrypted.seekg(DATA_START, ios_base::beg);
        encrypted.read(data, length);
        encrypted.close();
        cout << "Copied " << length << " bytes from offset 0x" << hex << DATA_START << endl;
    }
    else
    {
        decrypted.seekg(0, ios_base::beg);
        decrypted.read(data, length);
        decrypted.close();
        cout << "Copied " << length << " bytes from offset 0x0" << endl;
    }
    cout << "Done" << endl;

    // Decrypt data.
    cout << "Decrypting data..." << std::endl;
    for (size_t i = 0; i < length; ++i)
    {
        data[i] = data[i] ^ magic[i%MAGIC_LENGTH] ^ MAGIC_BYTE;
    }
    cout << "Done" << endl;

    // Write decrypted data to output file.
    cout << "Writing decrypted data to disk..." << endl;
    out.write(data, length);
    out.close();
    cout << "Done" << endl;

    delete [] data;
    data = NULL;

    return 0;
}
