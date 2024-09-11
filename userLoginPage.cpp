#include "userContactPage.cpp"                                                         //including the other file
using namespace std;    

#define LIMITER 72340172838076673;                                                    
//this number is to keep unsigned long long inside its limits of 2^64-1 in getHashCode(); it is (2^64-1)/(2^8-1) <---- the denominator is the maximum ASCII value a character can take                                                                                      
class loginData                                                                     //a class to return multiple values for attepmtLogin()
{
    public:
    bool loginSuccess;                                      
    string username;
    string password;
    loginData(bool lS, string u = "", string p = "")
    {
        loginSuccess = lS;
        username = u;
        password = p;
    }
};

unsigned long long getHashCode(string key)                                              
{//function which generates a hashCode for an entered username
    unsigned long long hashCode = 0;                                                    
    unsigned long long currCoeff = 1;
    int n = key.length();
    int i = n - 1;

    while ( i >= 0 )
    {
        hashCode += key[i]*currCoeff;                                                   //mutliplies face value of the ascii of current power with 37^x where x is the place
        hashCode %= LIMITER;                                                            //ex for string abc, its code will be 'a'*37^2 + 'b'*37^1 + 'c'*37^0 
        currCoeff *= 37;                                                                
        currCoeff %= LIMITER;

        i--;
    }
    return hashCode % LIMITER;                                                          // if n = n1*n2*n3, then n%r = ((n1%r)*(n2%r)*(n3%r))%r
}

void loadUsers(unordered_map<string, unsigned long long>* &userMap, unordered_map<unsigned long long, string>* &passMap)
{//a function which reads the username and password text files and creates 2 hashmaps corresponding to each
    string folderName = "./credentialInfo/";
    string userHash = "userHash.txt";
    string passHash = "passHash.txt";

    ifstream userInputFile(folderName+userHash, ios::in);               //creating a variable for "userHash.txt"
    ifstream passInputFile(folderName+passHash, ios::in);               //creating a variable for "passHash.txt", here ios::in signifies this is for reading data

    userMap = new unordered_map<string, unsigned long long>; 
    passMap = new unordered_map<unsigned long long, string>;

    if ( !userInputFile.is_open() )                                     //error handling just in case the username file does not open
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
                (*userMap)[user] = hash;                                                //storing hashCode for each username in the userMap hashmap
            }
        }
    }

    if ( !passInputFile.is_open() )                                 //error handling just in case password file does not open
    {
        cout<<"hashcode:password file cannot be opened..."<<endl;
    }
    else
    {
        while ( !passInputFile.eof() )                                                      //eof() returns true if we have reached the end of file
        {
            unsigned long long hash = 0;
            string pass = "";
            passInputFile>>hash;
            passInputFile>>pass;

            if ( hash != 0 )
            {
                (*passMap)[hash] = pass;                                                //storing passwords for each hashCode in the passMap hashmap
            }
        }
    }

    userInputFile.close();  
    passInputFile.close();                                          //closing the variable for both text files
}

void unloadUsers(unordered_map<string, unsigned long long>* &userMap, unordered_map<unsigned long long, string>* &passMap)
{//a function which writes into the username and password txt files from the hashmaps to reflect changes
    string folderName = "./credentialInfo/";
    string userHash = "userHash.txt";
    string passHash = "passHash.txt";

    ofstream userOutputFile(folderName+userHash, ios::trunc);                                       
    ofstream passOutputFile(folderName+passHash, ios::trunc);                                               //here, ios::trunc signifies that the files will be truncated first before being overwritten

    unordered_map<string, unsigned long long>::iterator itrUser = (*userMap).begin();                       //just for iterating through the tables and checking if the values have actually been stored
    while ( itrUser != (*userMap).end() )
    {
        string user;
        unsigned long long hash;
        user = (*itrUser).first;
        hash = (*itrUser).second;
        
        userOutputFile<<user<<" "<<hash<<endl;
        itrUser++;
    }

    unordered_map<unsigned long long, string>::iterator itrPass = (*passMap).begin();
    while ( itrPass != (*passMap).end() )
    {
        unsigned long long hash;
        string pass;
        hash = (*itrPass).first; 
        pass = (*itrPass).second;
        
        passOutputFile<<hash<<" "<<pass<<endl;
        itrPass++;
    } 

    delete userMap;
    userMap = NULL;

    delete passMap;                                                                                 //deleting the hashmaps
    passMap = NULL;

    userOutputFile.close();
    passOutputFile.close();                                                                        //closing the txt files
}

void viewUsers(unordered_map<string, unsigned long long>* userMap)
{//a function which prints the username of all the current users
    cout<<"Available users: "<<endl;
    unordered_map<string, unsigned long long>::iterator itrUser = (*userMap).begin();                       //creating an interator for the hashmap and printing all the names
    while ( itrUser != (*userMap).end() )
    {
        cout<< (*itrUser).first <<" ";
        itrUser++;
    } 
    cout<<endl<<endl;
}

loginData attemptLogin(unordered_map<string, unsigned long long>* &userMap, unordered_map<unsigned long long, string>* &passMap)
{//a function for login attempts. it uses loginData objects to return objects which contain information regarding login attempts
    string username;
    cout<<"Enter username: ";
    cin>>username;
    
    if ( !(*userMap).count(username) )                                                           //if the entered username does not exist in the userMap hashmap
    {
        cout<<"This username does not exist."<<endl;
    }
    else
    {
        unsigned long long hashcode = (*userMap)[username];                                    //retrieving the hashcode for the entered username
        string password;
        cout<<"Entered username exists."<<endl;
        cout<<"Enter password for this username: ";
        cin>>password;

        while (1)
        {
            if ( password != (*passMap)[hashcode] )                                         //if the entered password doesnot match with the password associated with the username
            {
                cout<<"The entered username and password do not match."<<endl;
            }
            else
            {
                cout<<"Successfully logged in!"<<endl;
                loginData couldLogin(true, username, password);                             //create an object of the loginData class with loginSuccess set to true
                return couldLogin;
            }
            cout<<"Re-enter password (0 to go back): ";
            cin>>password;

            if ( password == "0" )
            {
                break;
            }            
        }
    }

    loginData couldLogin(false);                                                           //creates an object with loginSuccess said to false
    return couldLogin;
}

void accountCreate(unordered_map<string, unsigned long long>* &userMap, unordered_map<unsigned long long, string>* &passMap)
{//a function which creates a new account
    cout<<"account creation..."<<endl<<endl;
    cout<<"___________________________________________________________________________________"<<endl;
    string username;
    cout<<"Enter username (0 to go back): ";
    cin>>username;

    if ( username != "0" )
    {
        unsigned long long hashCode = getHashCode(username);                            //generates a hashcode for the username

        if ( (*passMap).count(hashCode) )                                               //if that hashcode already exists
        {
            cout<<"This username cannot be used."<<endl;
        }
        else
        {
            string password;
            cout<<"Enter password (0 to go back): ";
            cin>>password;

            if ( password != "0" )
            {
                (*userMap)[username] = hashCode;                                        //creates a username:hashCode key value pair in the userMap hashmap
                (*passMap)[hashCode] = password;                                        //creates a hashCode:password key value pair in the passMap hashmap

                cout<<"Account successfully created."<<endl;
                
                string folderName = "./accessInfo/";
                string userAccess = username + "Access.txt";
    
                ofstream userAccessFile(folderName+userAccess, ios::trunc);
                if ( !userAccessFile.is_open() )
                {
                    cout<<"Unable to open the user access file"<<endl;
                }

                userAccessFile<<"0"<<" "<<"true"<<endl;
                userAccessFile<<"1"<<" "<<"false"<<endl;                              //setting the access condition for the new account to private
                
                cout<<"Access status for new accounts is set to private by default."<<endl;

                userAccessFile.close();
            }
            else
            {
                cout<<"cancelling account creation..."<<endl;        
            }
        } 
    }
    else
    {
        cout<<"cancelling account creation..."<<endl;
    }
}

void accountDelete(unordered_map<string, unsigned long long>* &userMap, unordered_map<unsigned long long, string>* &passMap)
{//a function to delete an existing account
    cout<<"account deletion..."<<endl<<endl;
    cout<<"___________________________________________________________________________________"<<endl;
    string username;
    cout<<"Enter username (0 to cancel): ";
    cin>>username;

    if ( username != "0" )
    {
        unsigned long long hashCode = getHashCode(username);

        if ( !(*userMap).count(username) )
        {
            cout<<"This username does not exist, it cannot be deleted."<<endl;
        }
        else
        {
            string password;
            cout<<"Enter password (0 to cancel): ";
            cin>>password;

            if ( password != "0" )
            {
                if ( password == (*passMap)[hashCode] )
                {
                    (*userMap).erase(username);
                    (*passMap).erase(hashCode);                                                     //deletes the key vaule pairs from both the hashmaps
                    cout<<"Credentials matched, account successfully deleted."<<endl;
                }
                else
                {
                    cout<<"Password does not match, cancelling account deletion."<<endl;
                }
            }
            else
            {
                cout<<"cancelling account deletion..."<<endl;        
            }
        } 
    }
    else
    {
        cout<<"cancelling account deletion..."<<endl;
    }
}

int userLoginPage()
{//the main function for this page
    unordered_map<string, unsigned long long>* userMap = NULL;
    unordered_map<unsigned long long, string>* passMap = NULL;
    loadUsers(userMap, passMap);                                                        //loads all the users from the txt files into hashmaps
    cout<<"users loaded..."<<endl;

    int opt = 1;
    while ( 1 )
    {
        cout<<"Enter: "<<endl;
        cout<<"0 to exit."<<endl;
        cout<<"1 to view available users."<<endl;
        cout<<"2 to login."<<endl;
        cout<<"3 to create an account."<<endl;
        cout<<"4 to delete an account."<<endl;
        cin>>opt;

        if ( opt == 0 )                                                                //calls the different functions based on user input
        {
            break;
        }
        else if ( opt == 1 )
        {
            viewUsers(userMap);
        }
        else if ( opt == 2 )
        {
            loginData loginCheck = attemptLogin(userMap, passMap);

            if ( loginCheck.loginSuccess )                                          //if the login attempt is successful then proceed to go to the contact page for that username
            {
                cout<<"Login successful."<<endl;
                unloadUsers(userMap, passMap);
                cout<<"users unloaded..."<<endl;
    
                cout<<"entering contact page..."<<endl;
                opt = userContactPage(loginCheck.username, loginCheck.password);
                
                loadUsers(userMap, passMap);                                        //loading the users again when user returns from the contact page to this page
                cout<<"users loaded..."<<endl;

            }
            else
            {
                cout<<"Login failed."<<endl;
            }
        }
        else if ( opt == 3 )
        {
            accountCreate(userMap, passMap);
        }
        else if ( opt == 4 )
        {
            accountDelete(userMap, passMap);
        }
        else
        {
            cout<<"work in progress..."<<endl;
        }
    }

    unloadUsers(userMap, passMap);
    cout<<"users unloaded..."<<endl;
    

    return opt;
}