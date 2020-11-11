/** @file
 *
 *  Author: Amey Chavan (apchavan)
 *  File description:-
 *  This is the decrypter.
 */

#include <iostream>
#include <string>
#include <getopt.h>
#include "MatImage.h"
#include "TextFile.h"
#include "Error.h"

using namespace std;
using namespace Steganography;

// Global variables
const string PROGRAM = "unsteg";

// Helper functions
void print_help();

//==================== main() ===================================================
int main(int argc, char** argv)
{
    // Declare necessary strings
    string image_filename;
    string text;
    string key;
    string out_filename;

    // Command-line options
    int option_index = 0;
    option long_options[] = {
        {"help",      no_argument,       0, 'h'},
        {"password",  required_argument, 0, 'p'},
        {"out-file",  required_argument, 0, 'o'},
        {0,0,0,0}
    };

    // Parse every options
    while (true)
    {
        int c = getopt_long(argc, argv, ":hp:o:", long_options, &option_index);

        // If parsing complete, break out of loop
        if (c == -1) break;

        switch (c)
        {
            case 'h':
                print_help();
                return 0;

            case 'p':
                key = string(optarg);
                break;

            case 'o':
                out_filename = string(optarg);
                break;

            case ':':
                error("Missing option argument");

            case '?':
                error("Unknown option");

            default:
                error("Unknown error");
        }   // 'switch (c)' closed.
    }   // 'while (true)' closed.

    // There should be exactly 1 non-option argument
    if ( (argc - optind) != 1 )
    {
        print_help();
        return 1;
    }

    // Open the image
    image_filename = string(argv[optind++]);
    MatImage I;
    try
    {
        I = image_filename;
    } 
    catch (const IOError& e)
    {
        error(e);
    }

    // Get the password
    if (key.empty()) 
    {
        cout << ":: Password: ";
        getline(cin, key);
    }

    // Unsteg
    TextFile file;
    try 
    {
        file = I.unsteg(key);

        if (out_filename.empty()) 
        {
            cout << file << endl;
        } 
        else 
        {
            file.save(out_filename);
            cout << ":: Hidden text extracted to '" << out_filename <<"'" << endl;
        }
    }
    catch (const exception& e) 
    {
        error(e);
    }

    return 0;
}   // 'int main(int argc, char** argv)' closed.

//======================== print_help() =======================================
void print_help()
{
    cout << "\n"
        "USAGE:\n"
        "  " << PROGRAM << " [OPTION...] IMAGE-FILE\n";

    cout << "\n"
        "OPTIONS:\n"
        "  -h, --help               display this help message\n"
        "  -p, --password=PASSWD    password to decrypt\n"
        "  -o, --out-file=FILE      output filename\n";
}   // 'print_help()' closed.
