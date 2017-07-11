#include <iostream>
#include <dirent.h>
#include <string>
#include <map>
#include <fstream>
#define _ translate

using namespace std;



string translate( string );
int loadTranslation( void );

map < string, string > i18n;


int main(){
    loadTranslation();
    cout << endl << "Output:" << endl;
    cout << "Translation test01 " << translate( "plate" ) << endl;
    cout << "Translation test02 " << translate( "mileagex" ) << endl; //Errorhandling
    cout << "Translation test03 with underline " << _( "mileage" ) << endl;

    return 0;

}

//get text form map
string translate( string msgid ){
    if( i18n.count( msgid ) )
        return i18n[msgid];
    else
        return "Error can't find \"" + msgid + "\" in language file.";
        //return msgid; //for development
}

// Load language data for language file in a map
int loadTranslation( void ){
    DIR *dpdf;
    string i18nPath =  "./languages/";
    string line, msgid, msgstr;
    bool msgid_status = false, msgstr_status = false;
    struct dirent *epdf;

    map < int, string > languages;

    int number = 0, selected_language = 0;
    dpdf = opendir( i18nPath.c_str() ); //read name of i18n-files

    cout << "Press a Number to select a language" << endl;
    while( epdf = readdir( dpdf ) ){
        if(  epdf->d_type == DT_DIR ) continue;
        ++number;
        string lng_file =  string( epdf->d_name ).substr( 0, string( epdf->d_name ).size() - 4 );
        cout << number << "\t" << lng_file << endl; //please put it in a 'drop down menu'
        languages.insert( make_pair( number, epdf->d_name ) );
    }
    cin >> selected_language;
    string filename = i18nPath + languages[selected_language];

    ifstream input( filename.c_str() );
    if( !input ){
        cerr << "Can't read:  " << filename << "\n";
        return -1;
    } 
    size_t found_first, found_last;

    int line_number = 0;
    while( getline( input, line ) ){
        line_number++;
        if( line.find( "#" ) != string::npos ) continue; //ignore comments
        if( line.find( "msgid" ) != string::npos  ){
            if( msgid_status ){
                cerr << "Syntax error in file " << filename << " near line: " << line_number << endl;
                return -1;
            }
            msgid_status = true;
            found_first = line.find( "\"", 5 );
            found_last = line.find( "\"", found_first + 1 );
            msgid = line.substr ( found_first + 1 , found_last - found_first - 1  );
        }
        if( line.find( "msgstr" ) != string::npos ){
            if( msgstr_status ){
                cerr << "Syntax error in file " << filename << " near line: " << line_number << endl;
                return -1;
            }
            msgstr_status = true;
            found_first = line.find( "\"", 6 );
            found_last = line.find( "\"", found_first + 1 );
            msgstr = line.substr ( found_first + 1 , found_last - found_first - 1 );
        }	
        if( msgid_status && msgstr_status ){
            msgid_status = msgstr_status = false;
            if( !i18n.count( msgid ) )
                i18n.insert( make_pair( msgid, msgstr ) );
            else{
                cerr << "Doublet error in file " << filename << " line: " << line_number - 1 << "." << endl;
                return -1;
            }
        }
    }//end while
}
