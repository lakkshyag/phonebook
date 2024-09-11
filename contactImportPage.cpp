class canAccessUser
{
    public:
    bool accessSuccess;
    string accessUsername;

    canAccessUser()
    {
        accessSuccess = false;
        accessUsername = "";
    }
    canAccessUser(bool aS, string aU="")
    {
        accessSuccess = aS;
        accessUsername = aU;
    }
};

canAccessUser accessUser(string username, unordered_map<string, unsigned long long>* &userMap)
{//a function to check if a particular user can be accessed by the current user
    cout<<endl;
    cout<<"Enter username to access: ";
    string accessUsername;
    cin>>accessUsername;

    bool canAccess = false;

    if ( username == accessUsername )
    {
        cout<<"You can already access your own contacts!"<<endl;
        canAccessUser failed;
        return failed;
    }

    if ( !(*userMap).count(accessUsername) )
    {
        cout<<"This username does not exist."<<endl;
        canAccessUser failed;   
        return failed;
    }
    else
    {
        string file = "./accessInfo/"+accessUsername+"Access.txt";
        ifstream userAccessFile(file, ios::in);
        
        unordered_map<string, bool>* userAccessMap = new unordered_map<string, bool>;

        if ( !userAccessFile.is_open() )
        {
            cout<<"access file cannot be opened..."<<endl;
        }
        else
        {
            while ( !userAccessFile.eof() )                                                        //eof() returns true if we have reached the end of file
            {
                string user = "";
                string access = "notSet";
                userAccessFile>>user;                                                              //reading values from files and storing them in variables
                userAccessFile>>access;

                if ( access != "notSet" )
                {
                    if ( access == "true")
                    {
                        (*userAccessMap)[user] = true;                                        //storing hash in the usernameTable with the key
                    }
                    else if ( access == "false" )
                    {
                        (*userAccessMap)[user] = false;
                    }                                                
                }
            }
            cout<<"access list loaded for "<<username<<"..."<<endl;

            if ( (*userAccessMap)["0"] == true )
            {
                cout<<accessUsername<<"'s access status set to private, can not access."<<endl;
                canAccess = false;
            }
            else if ( (*userAccessMap)["1"] == true )
            {
                cout<<accessUsername<<"'s access status set to public, can access."<<endl;
                canAccess = true;
            }
            else
            {
                if ( !(*userAccessMap).count(username) )
                {
                    cout<<"You are not on the access list, can not access."<<endl;
                    canAccess = false;
                }
                else if ( (*userAccessMap)[username] == false )
                {
                    cout<<"Your access condition to this account has been set to false, can not access."<<endl;
                    canAccess = false;
                }
                else if ( (*userAccessMap)[username] == true )
                {
                    cout<<"Your access condition to this account has been set to true, can access."<<endl;
                    canAccess = true;
                }
            }
        }

        userAccessFile.close();
        delete userAccessMap;
        cout<<"access file closed..."<<endl;
    }

    canAccessUser cAU(canAccess, accessUsername);
    return cAU;
}

canAccessUser contactImportPage(string username)
{//main function for this page
    unordered_map<string, unsigned long long>* userMap = NULL;
    loadUsersAccessPage(userMap);
    cout<<"other users loaded..."<<endl;

    cout<<endl<<"___________________________________________________________________________________"<<endl<<endl;
    cout<<"Contact import page for "<<username<<endl;
    
    int opt = 1;

    while ( opt )
    {
        cout<<"Enter"<<endl;
        cout<<"0 to go back."<<endl;
        cout<<"1 to view other available users."<<endl;
        cout<<"2 to access other user's contacts."<<endl;

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
            canAccessUser canAccess = accessUser(username, userMap);
            
            if ( canAccess.accessSuccess )
            {
                cout<<"This user can be accessed."<<endl;
                return canAccess;
            }
            else
            {
                cout<<"This user can not be accessed."<<endl;                
            }
        }
        else
        {
            //nothing
        }
    }

    canAccessUser noAccess;
    return noAccess;
}