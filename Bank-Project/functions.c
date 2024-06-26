typedef struct {
    int year;
    int month:5;
}date;
typedef struct {
    char acc_num[11];
    char name[30];
    char mobile[12];
    char e_addrs[35];
    double balance;
    date date_op;
}account;
account* data;
int n;
//Takes a string input form the user max 30 char convert it into lowercase and retarn a pointer
char* get_string(char* s)
{
    char* uinpt = malloc(30);
    printf("%s", s);
    //gets a string from the user max 30char
    fgets(uinpt, 30, stdin);
    //removes the enter from the string
    uinpt[strlen(uinpt)-1] = '\0';
    //convert the string into lowercase
    for (int i = 0; uinpt[i] != '\0'; i++)
        uinpt[i] = tolower(uinpt[i]);
    // removes any exess input from the user so it is not gets if i used scanf
    fflush(stdin);
    return uinpt;

}
// count the number of lines in text file
int recordsCount(char* l)
{
    FILE *ptr = fopen(l, "r");
    if (!ptr)
    {
        printf("Error file not found.\n");
        return 0;
    }
    char c;
    int i = 0;
    while ((c = getc(ptr)) != EOF) {
        if (c =='\n')
            i++;
    }
    return i;
}
int LOAD()
{
    int month, i;
    account a;
    FILE *op = fopen("accounts.txt", "r");
    if (!op)
    {
        printf("Error file not found.\n");
        return 0;
    }
    // scan a line and puts its data in an account
    for (i = 0; fscanf(op,"%11[^,],%30[^,],%35[^,],%lf,%12[^,],%d-%d\n", a.acc_num,a.name, a.e_addrs,
                       &a.balance, a.mobile, &month, &a.date_op.year) == 7; i++){
        a.date_op.month = month;
        // put the account in an array of accounts
        data[i] = a;
    }
    fclose(op);
    return i;
}
int LOGIN()
{
    char* uinpt;
    FILE* ptr = fopen("users.txt", "r");
    if (!ptr)
    {
        printf("Error file not found.\n");
        return 0;
    }
    // ask the user for his username
    uinpt = get_string("Enter the username: ");
    char str[30];
    long l;
    // scans aline reads what is befor the space and ignores what's after
    while (fscanf(ptr, "%s %*s\n", str) == 1)
    {
        // check if found a match is users.txt 
        if (!strcmp(uinpt , str))
        {
            // return the pointer to the start of the previews line
            fseek(ptr, l, SEEK_SET);
            // ignores what is before the space and takes what is after
            fscanf(ptr, "%*s %s", str);
            //chance 4 times to enter the password
            for (int i = 0; i<4; i++) {
                // i did not use get_string function when asking the user for the password becouse i want to leave the letters uppercase or lower case as enterd by the user
                printf("Enter password: ");
                fgets(uinpt, 30, stdin);
                uinpt[strlen(uinpt)-1] = '\0';
                if (!strcmp(uinpt, str)) {
                    fclose(ptr);
                    free(uinpt);
                    return 1;
                }
                else {
                    printf("Incorrect Password.\n");
                    printf("%d attempt left.\n", 3-i);
                }
            }
            fclose(ptr);
            free(uinpt);
            return 0;
        }
        //store the location of the current line so i can return back to to it if found username`
        l = ftell(ptr);
    }
    printf("Username not found\n");
    fclose(ptr);
    free(uinpt);
    return 0;
}
void SAVE()
{
    FILE* op = fopen("accounts.txt", "w");
    char month[3];
    month[2] = '\0';
    for (int i = 0; i<n; i++)
    {
        //convert the month to string so if th was 8 it will be stored like'08'of if was 11 will stay 2digits
        month[0] = '0' + (data[i].date_op.month/10);
        month[1] = '0' + (data[i].date_op.month%10);
        fprintf(op, "%s,%s,%s,%.2lf,%s,%s-%d\n",data[i].acc_num, data[i].name, data[i].e_addrs,
                data[i].balance,data[i].mobile , month, data[i].date_op.year);
    }
}
// print a given acount in specific format
void print_acc(account a)
{
    char* months[] = {"January", "February", "March", "April", "May", "June", "July",
                      "August" ,"September", "October", "November", "December"};
    printf("Account Number: %s\n", a.acc_num);
    printf("Name: %s\n", a.name);
    printf("E-mail: %s\n", a.e_addrs);
    printf("Balance: %.2f $\n", a.balance);
    printf("Mobile: %s\n", a.mobile);
    printf("Date Opened: %s %d\n\n", months[a.date_op.month-1], a.date_op.year);
}

void SEARCH()
{   int i,flag=0,found;
    int n=LOAD(); //uses load function to count number of accounts to be passed through while also creating the data array
    char uinpt[11];
    do{ //loop until valid entry is given
    printf("Please enter desired account number: ");
    scanf("%s", uinpt);
    for(i=0;i<10;i++){
        if(uinpt[i] >= '0' && uinpt[i] <= '9') flag=0; //checks if any character inputted is not a number
        else {flag=1; break;}}
    if(flag) printf("Invalid Entry\n");}
    while(flag);
    for(i=0;i<n;i++){
    if((strcmp(data[i].acc_num,uinpt))==0) { //searches for matching account number
            print_acc(data[i]); // prints found account
            found=1;
            break;}
    else found=0;
    }
    if (!found) printf("Account Not Found");
    }
