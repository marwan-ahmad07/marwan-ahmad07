#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
typedef struct {
    int year;
    int month:5;
}date;
typedef struct {
    char acc_num[11];
    char name[35];
    char mobile[12];
    char e_address[35];
    double balance;
    date date_op;
    int:3;
}account;
account* DATA;
int N = 0;
#define YELLOW_HL "\e[43m"
#define RESET_TEXT "\x1b[0m"
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
void swap(account *a, account* b)
{
    account c;
    c = *a;
    *a = *b;
    *b = c;
}
#pragma clang diagnostic pop
int dateCmp(account a, account b)
{
    double d1 = a.date_op.year + a.date_op.month/12.0, d2 = b.date_op.year + b.date_op.month/12.0;
    if (d1<d2)
        return 1;
    if (d1>d2)
        return -1;
    return 0;
}
int balanceCmp(account a, account b)
{
    if (a.balance == b.balance)
        return 0;
    if (a.balance < b.balance)
        return -1;
    return 1;
}
int nameCmp(account a, account b)
{
    return strcmp(a.name, b.name);
}

int recordsCount(char* location)
{
    FILE *ptr = fopen(location, "r");
    if (!ptr)
    {
        printf("Error file not found.\n");
        return 0;
    }
    char c;
    int i = 0;
    while ((c = (char)getc(ptr)) != EOF) {
        if (c =='\n')
            i++;
    }
    return i;
}
void str_Lowercase(char string[])
{   int i;
    for (i = 0; string[i] != '\0'; i++)
        string[i] = (char)tolower(string[i]);
}
char* get_string(char* output)
{
    char* user_input = malloc(50);
    if (!user_input) {
        printf("Sorry some thing went wrong.\n");
        exit(4);
    }
    printf("%s", output);
    fgets(user_input, 50, stdin);
    user_input[strlen(user_input) - 1] = '\0';
    str_Lowercase(user_input);
    fflush(stdin);
    return user_input;

}
int letters(const char* string)
{
    for (int i = 0; string[i] != '\0'; i++)
        if ((string[i] < 'a' || string[i] > 'z') && string[i] != ' ')
            return 0;
    return 1;
}
void excessSpaceFix(char *string){
    int i;
    int j;
    for (i = 0; string[i] == ' '; i++);
    string[i] = (char)toupper(string[i]);
    for (j = 0; string[j + i] != '\0'; j++)
        string[j] = string[j + i];
    string[j] = '\0';
}
int name_validity(char* userName) {
    if (userName[0] == ' ')
        excessSpaceFix(userName);
    userName[0] = (char)toupper(userName[0]);
    // Iterate through the string
    for (int i = 0; userName[i] != '\0'; i++) {
        if (!isalpha(userName[i]) && userName[i] != ' ') {
            printf("Name should be letters only\n");
            return 0;
        }
        if (userName[i] == ' ')
        {
            if (userName[i+1] == ' ')
                excessSpaceFix(&userName[i + 1]);
            else if (userName[i+1] == '\0')
                userName[i] = '\0';
            else
                userName[i+1] = (char)toupper(userName[i+1]);
        }
    }
    if (strlen(userName) > 35) {
        printf("Sorry maximum length allowed 35 character.\n");
        return 0;
    }
    return 1;
}
int accountNmValidity(char *accNm)
{
    if (strlen(accNm) != 10) {
        printf("Account number should be 10 digits.\n");
        return 0;
    }
    for (int i = 0; i<10; i++)
        if (accNm[i] < '0' || accNm[i] > '9') {
            printf("You should enter digits only.\n");
            return 0;
        }
    return 1;
}
int em_validity(char* email)
{   if (strlen(email) > 35) {
        printf("Sorry maximum length allowed 35 character.\n");
        return 0;
    }
    if (strchr(email, ' ') || strchr(email, ','))
        return 0;
    char* c;
    if (email[0] == '@' || email[0] == '.')
        return 0;
    if (!(c = strchr(email, '@')) || !strchr(c, '.'))
        return 0;
    if (c[1] == '.')
        return 0;
    for (int i = 1 ; c[i] != '\0' ; i++) {
        if (c[i] == '@')
            return 0;
        if (c[i] == '.' && (c[i+1] == '\0' || c[i+1] == '.'))
            return 0;
    }
    return 1;
}
int phoneNum_validity(char* phone)
{
    if (phone[1]-phone[0] != 1)
        return 0;
    if (strlen(phone) != 11)
        return 0;
    if (phone[2] < '0' || phone[2]> '2' && phone[2] !='5')
        return 0;
    for (int i = 3; i<11; i++)
    {
        if (phone[i]< '0' || phone[i]>'9')
            return 0;
    }
    return 1;
}
int findAcn(char* number)
{
    for (int i = 0; i < N; i++)
        if(!strcmp(DATA[i].acc_num, number))
            return i;
    return -1;
}
void get_name(char* n)
{
    char*user_input;
    user_input = get_string("Enter the name of the owner: ");
    while (!name_validity(user_input))
    {
        free(user_input);
        user_input = get_string("Enter the name of the user: ");
    }
    strcpy(n, user_input);
    free(user_input);
}
void get_phone(char* number)
{
    char* user_input;
    user_input = get_string("Enter the phone number: ");
    while (!phoneNum_validity(user_input)) {
        printf("Invalid phone number\n");
        free(user_input);
        user_input = get_string("Enter the phone number: ");
    }
    strcpy(number, user_input);
    free(user_input);

}
void get_email(char* email)
{
    char* user_input;
    user_input = get_string("Enter the email-mail address: ");
    while (!em_validity(user_input)) {
        free(user_input);
        printf("Invalid email address\n");
        user_input = get_string("Enter the email-mail address: ");
    }
    strcpy(email, user_input);
    free(user_input);
}
double get_balance()
{
    char buf[15];
    int flag;
    int d ;
    double m;
    do{
        d = 0;
        flag = 0;
        printf("Enter the account balance: ");
        fgets(buf, 15, stdin);
        buf[strlen(buf) - 1] = '\0';
        for (int i = 0; buf[i] != '\0'; i++) {
            if ((buf[i] < '0' || buf[i] > '9') && buf[i] != '.') {
                flag = 1;
                printf("Enter numbers only\n");
                fflush(stdin);
                break;
            }
            if (buf[i] == '.' && !d)
                d = 1;
            else if (buf[i] == '.' && d)
            {
                printf("Invalid number\n");
                flag = 1;
                break;
            }

        }
    } while (flag);
    fflush(stdin);
    m = strtod(buf, NULL);
    return m;

}
void bubbleSort(int (*sort)(account, account), int n) // NOLINT(*-no-recursion)
{
    int i ,c = 0;
    for (i = 0 ; i < n - 1; i++)
    {
        if (sort(DATA[i], DATA[i+1])== 1)
        {
            c = 1;
            swap(&DATA[i], &DATA[i + 1]);
        }
    }
    if(c)
        bubbleSort(sort, n-1);
}
void print_acc(account a, char *word, int p)
{
    char* months[] = {"January", "February", "March", "April", "May", "June", "July",
                      "August" ,"September", "October", "November", "December"};
    printf("\n\tAccount Number: %s\n", a.acc_num);
    printf("\tName: ");
    if (!word)
        printf("%s\n", a.name);
    else
    {
        int lw = (int)strlen(word);
        for (int h = 0; h < p;h++)
            printf("%c", a.name[h]);
        if (!p || a.name[p -1] == ' ')
            word[0] = (char)toupper(word[0]);
        printf( YELLOW_HL"%s" RESET_TEXT, word);
        word[0] = (char)tolower(word[0]);
        printf("%s\n", a.name+(p+lw));
    }
    printf("\tE-mail: %s\n", a.e_address);
    printf("\tBalance: %.2f $\n", a.balance);
    printf("\tMobile: %s\n", a.mobile);
    printf("\tDate Opened: %s %d\n\n", months[a.date_op.month-1], a.date_op.year);
}
account* getAccount()
{
    char* user_input;
    int i;
    while (1) {
        user_input = get_string("Enter account number:");
        if (!strcmp(user_input, "exit")) {
            free(user_input);
            return NULL;
        }
        if (!accountNmValidity(user_input)) {
            free(user_input);
            continue;
        }
        if ((i = findAcn(user_input)) != -1) {
            free(user_input);
            return &DATA[i];
        }
        printf("Account Not found\n");
        free(user_input);
    }
}
int yesOrNo(char* output)
{
    char* conf;
    while (1) {
        conf = get_string(output);
        if (!strcmp(conf, "yes")) {
            free(conf);
            return 1;
        }
        if (!strcmp(conf, "no")) {
            free(conf);
            return 0;
        }
        printf("The answer should be yes or no.\n");
        free(conf);
    }

}
void SAVE();
void money_transfer(char mode)
{
    account* a;
    char* s;
    if (mode == 'd')
        s = "deposit";
    else
        s = "withdraw";
    a = getAccount();
    if (!a)
        return;
    print_acc(*a,NULL,0);
    double m = 0;
    char f;
    do
    {
        printf("Enter the amount of money you want to %s: ", s);
        scanf("%15lf%c", &m, &f); // NOLINT(*-err34-c)
        if (m<=0 || f != '\n')
            printf("Invalid input.\n");
        else if (m > 10000) {
            m = 0;
            printf("Sorry you can't %s more than 10,000$.\n", s);
        }
        else if (mode == 'w' && a->balance < m) {
            m = 0;
            printf("Sorry no enough money in balance.\n");
        }
        fflush(stdin);
    } while (!m);

    if (yesOrNo("Do you want to complete the transaction YES/NO?: ")) {
        if (mode == 'w')
            m *= -1;
        a->balance += m;
        printf("Transaction done successfully.\n");
        char str[30];
        sprintf(str, "History/%s.txt", a->acc_num);
        FILE *pta = fopen(str, "a");
        if (!pta)
            return;
        if (mode == 'd')
            fprintf(pta, "Deposited by %.2lf $.\n", m);
        else
            fprintf(pta, "Withdraw by %.2lf $.\n", m * -1);
        fclose(pta);
        SAVE();
    } else printf("Canceled.\n");

}
int confirmation(){
    char* confirm;
    while (1){
        confirm = get_string("- MENU\n- QUIT\n");
        if (!strcmp(confirm, "menu")) {
            free(confirm);
            return 1;
        }
        if (!strcmp(confirm, "quit")) {
            free(confirm);
            return 0;
        }
        printf("Please select one of the options above.\n");
        free(confirm);
    }
}
void TRANSFER();
void REPORT();
void MODIFY();
void WITHDRAW();
void DEPOSIT();
void DELETE();
void PRINT();
void SEARCH();
void ADV_SEARCH();
int LOGIN();
int LOAD();
void ADD();
void MENU(int i);
int main() {
    printf("""         _._._                       _._._\n"
                   "        _|   |_                     _|   |_\n"
                   "        | ... |_._._._._._._._._._._| ... |\n"
                   "        | ||| | o M M M B H BANK o  | ||| |\n"
                   "        | \"\"\" |  \"\"\"    \"\"\"    \"\"\"  | \"\"\" |\n"
                   "   ())  |[-|-]| [-|-]  [-|-]  [-|-] |[-|-]|  ())\n"
                   "  (())) |     |---------------------|     | (()))\n"
                   " (())())| \"\"\" |  \"\"\"    \"\"\"    \"\"\"  | \"\"\" |(())())\n"
                   " (()))()|[-|-]|  :::   .-\"-.   :::  |[-|-]|(()))()\n"
                   " ()))(()|     | |~|~|  |_|_|  |~|~| |     |()))(()\n"
                   "    ||  |_____|_|_|_|__|_|_|__|_|_|_|_____|  ||\n"
                   " ~ ~^^ @@@@@@@@@@@@@@/=======\\@@@@@@@@@@@@@@ ^^~ ~\n"
                   "      ^~^~                                ~^~^\n""");
    int lgn = 0;
    char* cmd;
    printf("\t\tWelcome to the bank\n");
    while (lgn != 1)
    {
        MENU(lgn);
        lgn = 0;
        cmd = get_string("Enter a command: ");
        if (!strcmp(cmd, "login"))
            lgn = LOGIN();
        else if (!strcmp(cmd, "quit"))
            break;
        else {
            printf("Invalid command.\n");
            lgn = -1;
        }
        free(cmd);
    }
    if (lgn)
    {
        system("cls");
        printf("Welcome\n");
        DATA = malloc(sizeof(account) * (recordsCount("accounts.txt") + 100));
        if (!DATA)
            return 3;
        N = LOAD();
    }

    while(lgn)
    {   MENU(lgn);
        cmd = get_string("Enter a command: ");
        if (!strcmp(cmd, "add"))
            ADD();
        else if (!strcmp(cmd, "delete"))
            DELETE();
        else if (!strcmp(cmd, "modify"))
            MODIFY();
        else if (!strcmp(cmd, "search"))
            SEARCH();
        else if (!strcmp(cmd, "advanced search"))
            ADV_SEARCH();
        else if (!strcmp(cmd, "withdraw"))
            WITHDRAW();
        else if (!strcmp(cmd, "deposit"))
            DEPOSIT();
        else if (!strcmp(cmd, "transfer"))
            TRANSFER();
        else if (!strcmp(cmd, "report"))
            REPORT();
        else if (!strcmp(cmd, "print"))
            PRINT();
        else if (!strcmp(cmd, "quit")) {
            free(cmd);
            break;
        }
        else {
            printf("Invalid command\n");
            lgn = -1;
            continue;
        }
        free(cmd);
        if(!confirmation())
            break;
        lgn = 1;
        system("cls");
    }
    SAVE();
    free(DATA);
    printf("BYE.");
    return 0;
}
int LOAD()
{
    int month, i;
    account a;
    FILE *op = fopen("accounts.txt", "r");
    if (!op)
    {
        printf("Error file not found.\n");
        exit(2);
    }
    // scan a line and puts its data in an account
    for (i = 0; fscanf(op,"%11[^,],%30[^,],%35[^,],%lf,%12[^,],%d-%d\n", a.acc_num,a.name, a.e_address, // NOLINT(*-err34-c)
                       &a.balance, a.mobile, &month, &a.date_op.year) == 7; i++){
        a.date_op.month = month;
        // put the account in an array of accounts
        DATA[i] = a;
    }
    fclose(op);
    return i;
}
int LOGIN() // NOLINT(*-no-recursion)
{
    char* user_input;
    FILE* ptr = fopen("users.txt", "r");
    if (!ptr)
    {
        printf("Error file not found.\n");
        exit(1);
    }
    // ask the user for his username
    user_input = get_string("Enter the username: ");
    if (!strcmp(user_input, "exit"))
        return 0 ;
    char str[30];
    int l = 0;
    // scans a line reads what is before the space and ignores what's after
    while (fscanf(ptr, "%s %*s\n", str) == 1)
    {
        // check if found a match is users.txt
        if (!strcmp(user_input , str))
        {
            // return the pointer to the start of the previews line
            fseek(ptr, l, SEEK_SET);
            // ignores what is before the space and takes what is after
            fscanf(ptr, "%*s %s", str);
            //chance 4 times to enter the password
            for (int i = 0; i<4; i++) {
                // I did not use get_string function when asking the user for the password because I want to leave the
                // letters uppercase or lower case as entered by the user
                printf("Enter password: ");
                fgets(user_input, 30, stdin);
                user_input[strlen(user_input) - 1] = '\0';
                if (!strcmp(user_input, str)) {
                    fclose(ptr);
                    free(user_input);
                    return 1;
                }
                else {
                    printf("Incorrect Password.\n");

                    printf("%d attempt left.\n", 3-i);
                }
            }
            fclose(ptr);
            free(user_input);
            return 0;
        }
        //store the location of the current line so  can return back to it if found username`
        l = ftell(ptr);
    }
    printf("Username not found\n");
    fclose(ptr);
    free(user_input);
    return LOGIN();
}
void SAVE()
{
    if (!N)
        return;
    FILE* op = fopen("accounts.txt", "w");
    char month[3];
    month[2] = '\0';
    for (int i = 0; i<N; i++)
    {
        //convert the month to string so if th was 8 it will be stored like'08'of if was 11 will stay 2digits
        month[0] = (char)('0' + (DATA[i].date_op.month/10));
        month[1] = (char)('0' + (DATA[i].date_op.month%10));
        fprintf(op, "%s,%s,%s,%.2lf,%s,%s-%d\n",DATA[i].acc_num, DATA[i].name, DATA[i].e_address,
                DATA[i].balance,DATA[i].mobile , month, DATA[i].date_op.year);
    }
    fclose(op);
}
void MENU(int i)
{
    if (i == -1)
        return;
    printf("MENU:\n");
    if (!i)
        printf("\t- LOGIN\n");
    else
        printf("\t- ADD\n"
               "\t- DELETE\n"
               "\t- MODIFY\n"
               "\t- SEARCH\n"
               "\t- ADVANCED SEARCH\n"
               "\t- WITHDRAW\n"
               "\t- DEPOSIT\n"
               "\t- TRANSFER\n"
               "\t- REPORT\n"
               "\t- PRINT\n");
    printf("\t- QUIT\n");
}

void ADD()
{
    system("cls");
    account a;
    char* user_input;
    int flag = 1;
    do{
        user_input = get_string("Enter a new account number: ");
        if(!strcmp(user_input, "exit")) {
            free(user_input);
            return;
        }
        if(!accountNmValidity(user_input))
            free(user_input);

        else if (findAcn(user_input) != -1)
        {
            printf("Account already taken\n");
            free(user_input);
        }
        else
            flag = 0;
    }while(flag);

    strcpy(a.acc_num, user_input);
    free(user_input);
    get_name(a.name);
    get_phone(a.mobile);
    get_email(a.e_address);
    a.balance = get_balance();
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    a.date_op.month = tm.tm_mon +1;
    a.date_op.year = tm.tm_year+1900;
    print_acc(a, NULL, 0);

    if (yesOrNo("Do to save the new account? YES / NO: "))
    {
        DATA[N] = a;
        N ++;
        SAVE();
        printf("Account saved\n");
        return;
    }
    printf("New account not saved\n");
}
void SEARCH()
{
    system("cls");
    account* a;
    a = getAccount();
    if (!a)
        return;
    print_acc(*a, NULL, 0);

}
void ADV_SEARCH()
{
    system("cls");
    int i, flag, p,found=0;
    char* user_input;
    char* keyword;
    char buffer[30];
    do{
        flag = 0;
        user_input = get_string("Please enter keyword: ");
        if (!strcmp(user_input, "exit"))
            return;
        if (!letters(user_input))
        {
            printf("The keyword must contain letters only\n");
            flag = 1;
            free(user_input);
        }
    }while(flag);
    printf("Search results: \n");
    for(i = 0 ; i < N; i++){
        strcpy(buffer, DATA[i].name);
        str_Lowercase(buffer);
        keyword= strstr(buffer, user_input);
        if(keyword != NULL){
            found++;
            p =(int)(keyword - buffer);
            printf("Account %d: \n", found);
            print_acc(DATA[i], user_input, p);
        }
    }
    if(!found)
        printf("\tNo matches found\n");
    free(user_input);
}
void PRINT()
{
    system("cls");
    char* user_input;
    int flag;
    do{
        flag = 0;
        user_input = get_string("Sort by: NAME / BALANCE / DATE OPENED\n");
        if (!strcmp(user_input, "name"))
            bubbleSort(nameCmp, N);
        else if (!strcmp(user_input, "balance"))
            bubbleSort(balanceCmp, N);
        else if (!strcmp(user_input, "date opened"))
            bubbleSort(dateCmp, N);
        else if (!strcmp(user_input, "exit")) {
            free(user_input);
            return;
        }
        else
        {
            printf("Please select one of the options above.\n");
            flag = 1;
            free(user_input);
        }

    } while (flag);
    for (int i = 0; i<N; i++)
        print_acc(DATA[i], NULL, 0);
}
void DELETE()
{
    system("cls");
    account* a = getAccount();
    if (!a)
        return;
    print_acc(*a,NULL,0);
    if (a->balance) {
        printf("Deletion of Bank account is only allowed for zero balance accounts.\n"
               "Deletion rejected.\n");
        return;
    }
    if (yesOrNo("Are you sure you want to delete this account? YES / NO: "))
    {
        swap(a, &DATA[N - 1]);
        N-=1;
        SAVE();
        printf("Account deleted\n");
        return;
    }
    printf("Account not deleted.\n");
}
void DEPOSIT()
{
    system("cls");
    money_transfer('d');
}
void WITHDRAW()
{
    system("cls");
    money_transfer('w');
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
void MODIFY()
{
    system("cls");
    account *a;
    account temp;
    int c = 0;
    a = getAccount();
    if (!a)
        return;
    print_acc(*a, NULL, 0);
    temp = *a;
    if (yesOrNo("Do you want to edit the Name Yes/No?: ")) {
        get_name(temp.name);
        c = 1;
    }
    if (yesOrNo("Do you want to edit the phone number Yes/No?: ")) {
        get_phone(temp.mobile);
        c = 1;
    }
    if (yesOrNo("Do you want to edit the Email Yes/No?: ")) {
        get_email(temp.e_address);
        c = 1;
        }
    if (c)
    {
        print_acc(temp, NULL, 0);
        if (yesOrNo("Do you want to save the changes? YES / NO: "))
        {
            *a = temp;
            SAVE();
            printf("Modifications saved\n");
            return;
        }
        printf("Changes not saved.\n");
    }
}
#pragma clang diagnostic pop
void REPORT()
{
    system("cls");
    char* user_input;
    int position[5], flag;
    do{
        flag = 0;
        user_input = get_string("Enter the account number: ");
        if (!strcmp(user_input, "exit"))
            return;
        if (!accountNmValidity(user_input))
            flag = 1;
        else if (findAcn(user_input) == -1) {
            printf("Account not found\n");
            flag = 1;
        }
    } while (flag);
    char str[30];
    sprintf(str, "History/%s.txt", user_input);
    FILE *ptr = fopen(str, "r");
    if (!ptr) {
        printf("This account has no history\n");
        return;
    }
    char line[200];
    int l;
    int i, j, k;
    l = recordsCount(str);
    if (l<5)
        i = 0;
    else
        i = l-5;
    k = 0;
    printf("Account Report: \n");
    for (j = 0; fgets(line, 200, ptr) != NULL;j++ )
    {
        if (j>=i)
            position[j-i] = k;
        k = ftell(ptr);
    }
    for (i = j-i-1; i >= 0; i--)
    {
        fseek(ptr, position[i], SEEK_SET);
        fgets(line, 200, ptr);
        printf("%s\n", line);
    }
    fclose(ptr);
    free(user_input);
}

void TRANSFER()
{
    system("cls");
    account* sender;
    account* reciever;
    double amount = 0;
    char f;
    int flag = 1;
    printf("Please enter the sender account number\n");
    sender= getAccount();
    if (!sender)
        return;
    print_acc(*sender,NULL,0);
    do {
        printf("Please enter the reciever account number\n");
        reciever = getAccount();
        if (!reciever)
            return;
        if (sender == reciever)
            printf("Please enter an account number other than the one you entered in the sender.\n");
    } while (sender == reciever);
    print_acc(*reciever,NULL,0);
    do{
        printf("Please enter the amount to be transferred: ");
        scanf("%15lf%c",&amount, &f); // NOLINT(*-err34-c)
        fflush(stdin);
        if (amount<=0 || f != '\n')
            printf("Invalid input\n");

        else if(sender->balance < amount)
            printf("Sorry not enough money in balance\n");

        else
            flag=0;

    }while(flag);
    if(yesOrNo("Do You want to complete the transaction? YES/NO\n")) {
        sender->balance-=amount;
        reciever->balance+=amount;
        SAVE();
        printf("Transaction Done Successfully\n");
        char str[30];
        sprintf(str ,"History/%s.txt", sender->acc_num);
        FILE *pta = fopen(str, "a");
        fprintf(pta,"Sent %.2lf $ to %s account number: %s.\n", amount, reciever->name, reciever->acc_num);
        fclose(pta);
        sprintf(str ,"History/%s.txt", reciever->acc_num);
        pta = fopen(str, "a");
        fprintf(pta,"Received %.2lf $ from %s account number: %s.\n", amount, sender->name, sender->acc_num);
        fclose(pta);
    }
    else
        printf("Transfer Cancelled\n");
}
