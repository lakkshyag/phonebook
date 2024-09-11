#include <iostream>                                                          //importing the neccessary library and the other files
#include "userLoginPage.cpp"
#include "welcome.cpp"
#include "goodbye.cpp"                                                        

using namespace std;

int main()                                                                 //main file for the program
{
    
    int opt = 2;                                                          //here any value for opt would work except for 1 and 0
    while ( opt )
    {
        if ( opt == 2 )                                                   //declared value for opt should be same as the value being 
        {                                                                 //checked here so user goes straight to the welcome page
            cout<<"entering welcome page..."<<endl;
            cout<<"___________________________________________________________________________________"<<endl;
            opt = welcomePage();
        }
        else if ( opt == 1 )                                              //if welcome page returns with 1, it directly takes user to the login page
        {
            cout<<"entering login page..."<<endl;
            cout<<"___________________________________________________________________________________"<<endl;
            opt = userLoginPage();
        }
        else 
        {
            //nothing
        }     
    }    

    if ( opt )                                                           //error handling just in case
    {
        cout<<"premature exit, goodbyepage not called";
    }
    else
    {
        cout<<"entering goodbye page..."<<endl;                          //takes uesr to goodbye page if they return with welcome page or user login page with 0
        cout<<"___________________________________________________________________________________"<<endl;
        opt = goodbyePage();
    }

    return 0;
}