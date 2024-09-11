#include <unordered_map>                                                                    //library which contains hashmaps
#include <fstream>
using namespace std;

void loadUsersAccessPage(unordered_map<string, unsigned long long>* &userMap)
{//a funtion which loads all the usernames from the userHash.txt file into a hashmap userMap
    string folderName = "./credentialInfo/";
    string userHash = "userHash.txt";

    ifstream userInputFile(folderName+userHash, ios::in);
    userMap = new unordered_map<string, unsigned long long>; 

    if ( !userInputFile.is_open() )
    {
        cout<<"username:hashcode file cannot be opened..."<<endl;
    }
    else
    {
        while ( !userInputFile.eof() )                                                        //eof() returns true if we have reached the end of file
        {
            string user = "";
            unsigned long long hash = 0;
            userInputFile>>user;                                                              //reading values from files and storing them in variables
            userInputFile>>hash;

            if ( hash != 0 )
            {
                (*userMap)[user] = hash;                                                //storing hash in the usernameTable with the key
            }
        }
    }
    userInputFile.close();
}

void loadAccess(string username, unordered_map<string, bool>* &userAccessMap)
{//loads the access file into a hashmap from the txt file
    string folderName = "./accessInfo/";
    string userAccess = username + "Access.txt";

    ifstream userAccessFile(folderName+userAccess, ios::in);
    userAccessMap = new unordered_map<string, bool>; 

    if ( !userAccessFile.is_open() )
    {
        cout<<"access file cannot be opened..."<<endl;
    }
    else
    {
        while ( !userAccessFile.eof() )                                                        
        {
            string user = "";
            string access = "notSet";
            userAccessFile>>user;                                                              
            userAccessFile>>access;

            if ( access != "notSet" )
            {
                if ( access == "true")
                {
                    (*userAccessMap)[user] = true;                                        
                }
                else if ( access == "false" )
                {
                    (*userAccessMap)[user] = false;
                }                                                
            }
        }
    }
    
    userAccessFile.close();    
}

void unloadAccess(string username, unordered_map<string, bool>* &userAccessMap)
{//writes into the access txt from the hashmap
    string folderName = "./accessInfo/";
    string userAccess = username + "Access.txt";

    ofstream userAccessFile(folderName+userAccess, ios::trunc);


    unordered_map<string, bool>::iterator itrUser = (*userAccessMap).begin();                       //just for iterating through the tables and checking if the values have actually been stored
    while ( itrUser != (*userAccessMap).end() )
    {
        string user;
        bool canAccess;
        user = (*itrUser).first;
        canAccess = (*itrUser).second;
        string access;

        if ( canAccess )
        {
            access = "true";
        }
        else
        {
            access = "false";
        }

        userAccessFile<<user<<" "<<access<<endl;
        itrUser++;
    }

    delete userAccessMap;
    userAccessMap = NULL;

    userAccessFile.close();
}

void viewUsersAccessPage(string username, unordered_map<string, unsigned long long>* userMap)
{//view all the available users (except yourself)
    cout<<endl<<"Available users: "<<endl;
    unordered_map<string, unsigned long long>::iterator itrUser = (*userMap).begin();                       
    while ( itrUser != (*userMap).end() )
    {
        if ( username == (*itrUser).first )
        {
            itrUser++;
            continue;
        }
        cout<< (*itrUser).first <<" ";
        itrUser++;
    } 
    cout<<endl;
}

void viewAccessStatus(unordered_map<string, bool>* userAccessMap)
{//a function for viewing your current access condition and cekcing which other users can access your contacts
    cout<<endl<<"Current access condition: "<<endl;
    if ( (*userAccessMap)["0"] == true )
    {
        cout<<"Private."<<endl;
        return;
    }
    else if ( (*userAccessMap)["1"] == true )
    {
        cout<<"Public."<<endl;
        return;
    }
    else 
    {
        cout<<"With users: "<<endl;
        unordered_map<string, bool>::iterator itrUser = (*userAccessMap).begin();
        while ( itrUser != (*userAccessMap).end() )
        {
            if ( (*itrUser).second )
            {
                cout<<(*itrUser).first<<" ";
            }
            itrUser++;
        }
        cout<<endl;
    }   
}

void provideAccess(unordered_map<string, unsigned long long>* userMap, unordered_map<string, bool>* userAccessMap, string usernameHost)
{//a function to give access to a user
    string username;
    cout<<endl<<"Enter username to give access to (0 to go back): ";
    cin>>username;

    if ( username == "0" )
    {
        return;
    }
    else if ( username == usernameHost )
    {
        cout<<"Cannot give access to yourself!!"<<endl;
        return;
    }
    else
    {
        if ( !(*userMap).count(username) )
        {
            cout<<"This username does not exist."<<endl;
            return;
        }
        
        if ( (*userAccessMap)[username] )
        {
            cout<<"You have already provided this user with access."<<endl;
        }
        else
        {
            cout<<"Access provided to "<<username<<endl;
            (*userAccessMap)[username] = true;
        }
        
        (*userAccessMap)["1"] = false;
        (*userAccessMap)["0"] = false;
        return;
    }
}

void revokeAccess(unordered_map<string, unsigned long long>* userMap, unordered_map<string, bool>* userAccessMap, string usernameHost)
{//function to revoke access from a user
    string username;
    cout<<endl<<"Enter username to revoke access from (0 to go back): ";
    cin>>username;

    if ( username == "0" )
    {
        return;
    }
    else if ( username == usernameHost )
    {
        cout<<"Cannot revoke access from yourself!!"<<endl;
        return;
    }
    else
    {
        if ( !(*userMap).count(username) )
        {
            cout<<"This username does not exist."<<endl;
            return;
        }
        
        if ( !(*userAccessMap)[username] )
        {
            cout<<"This user does not have access."<<endl;
        }
        else
        {
            cout<<"access revoked from "<<username<<endl;
            (*userAccessMap)[username] = false;
        }
        return;
    }
}

void makePublic(unordered_map<string, bool>* userAccessMap)
{//function to make account public
    if ( (*userAccessMap)["1"] )
    {
        cout<<"Account is already public."<<endl;
    }
    else
    {
        (*userAccessMap)["1"] = true;
        (*userAccessMap)["0"] = false;
        cout<<"Account made public."<<endl;
    }
}

void makePrivate(unordered_map<string, bool>* userAccessMap)
{//function to make account private
    if ( (*userAccessMap)["0"] )
    {
        cout<<"Account is already private."<<endl;
    }
    else
    {
        (*userAccessMap)["0"] = true;
        (*userAccessMap)["1"] = false;
        cout<<"Account made private."<<endl;
    }
}

int userAccessProvidePage(string username)
{
    unordered_map<string, unsigned long long>* userMap = NULL;
    loadUsersAccessPage(userMap);
    cout<<"other users loaded..."<<endl;

    unordered_map<string, bool>* userAccessMap = NULL;
    loadAccess(username, userAccessMap);
    cout<<"access file for user loaded..."<<endl;
    
    cout<<endl<<"___________________________________________________________________________________"<<endl<<endl;
    cout<<"Access control page for user: "<<username<<endl;

    int opt = 1;
    while ( opt )
    {
        cout<<"Enter: "<<endl;
        cout<<"0 to go back."<<endl;
        cout<<"1 to view other users"<<endl;
        cout<<"2 to provide access to a user."<<endl;
        cout<<"3 to revoke access from a user."<<endl;
        cout<<"4 to make account public (accessible to all)."<<endl;
        cout<<"5 to make account private (accessible to none)."<<endl;
        cout<<"6 to view current access status."<<endl;

        cin>>opt;
        cin.ignore(1, '\n');

        if ( opt == 0 )
        {
            break;
        }
        else if ( opt == 1 )
        {
            viewUsersAccessPage(username, userMap);
        }
        else if ( opt == 2 )
        {
            provideAccess(userMap, userAccessMap, username);
        }
        else if ( opt == 3 )
        {
            revokeAccess(userMap, userAccessMap, username);
        }
        else if ( opt == 4 )
        {
            makePublic(userAccessMap);
        }
        else if ( opt == 5 )
        {
            makePrivate(userAccessMap);
        }
        else if ( opt == 6 )
        {
            viewAccessStatus(userAccessMap);
        }
        else
        {
            //nothing
        }
    }

    unloadAccess(username, userAccessMap);
    cout<<"access page for "<<username<<" unloaded..."<<endl;

    return 1;
}

