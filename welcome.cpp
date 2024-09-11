#include <sys/stat.h>
using namespace std;

int welcomePage()
{
    cout<<"Welcome to the Contact Book"<<endl<<endl;                    //basic landing page
    cout<<"Enter: "<<endl;
    cout<<"0 to exit"<<endl;
    cout<<"1 to proceed to user login page"<<endl;
    int opt = 1;
    cin>>opt;

    if ( !opt )                                                        //if user enters 0, then they return to the main page with the value 0
    {                                                                  //since user returns with 0, they head to the goodbye page
        return 0;
    }
    else
    {
        cout<<"creating necessary directories..."<<endl;               //if user enters anything else creating the required directories or verifying their existence
        int check;

        const char* dirname = "credentialInfo";
        check = mkdir(dirname);
        if (!check)
        {
            cout<<dirname<<" folder created...";
        }
        else
        {
            cout<<dirname<<" folder already exists...";
        }
        cout<<endl;
        
        dirname = "contactInfo";
        check = mkdir(dirname);
        if (!check)
        {
            cout<<dirname<<" folder created...";
        }
        else
        {
            cout<<dirname<<" folder already exists...";
        }
        cout<<endl;

        dirname = "accessInfo";
        check = mkdir(dirname);
        if (!check)
        {
            cout<<dirname<<" folder created...";
        }
        else
        {
            cout<<dirname<<" folder already exists...";
        }
        cout<<endl;
    }

    opt = 1;
    return opt;
}