# contact
C Language Program: Contact Management

## Description
Create and save a person contact on terminal.
> This program has features:
1. Add new contact
2. Search contact
3. Edit contact
4. List of contact
5. Delete contact

## How the program works?
* The program use `.csv` file to save every contact you want to save. It will load `contacts.csv` file everytime you run the program. If you want to renew your `contacts.csv` file, you can delete the `.csv` file itself and create new one. Or you can just delete all rows in `.csv` file (except the header title).
  
  > Remember to create `contacts.csv` file to run this program on root directory

* The program use `hash table` to store all contacts in `contacts.csv` file the first time it loaded. This is part of my training about `hash table algorithm`, in which will help to perform faster when we want to check one person contact details or get all list of contacts
* The program use simple command line that has been setting in `Makefile`. We can run and clean it just with `Make` command. If you ever modify the code, before you run it you must delete all the `compiled` `.o` file with `Make clean`.

  > Please read the Makefile for more understanding

## Requirements for using it
* A software (IDE) for reviewing the code
* C/C++ compiler (Recommended: GCC Compiler)
* Command Prompt or Windows Powershell
* Install CS50 Library. Instruction at [CS50 Library for C](https://cs50.readthedocs.io/libraries/cs50/c/)

## How to run the program?
1. Run with: `make contact`
2. Next, run: `./contact`
3. It will draw you the menu of all features

## Screenshots
### 1. Menu
![Screenshot (40)](https://github.com/user-attachments/assets/c23d26ec-a031-4653-9093-b041b43fa814)
### 2. Add New Contact
![Screenshot (42)](https://github.com/user-attachments/assets/c6e6cb8c-60f8-4b2a-a60b-9c405a01da09)
### 3. Search Contact
  * Search by name
    
    ![Screenshot (43)](https://github.com/user-attachments/assets/b566addd-3aa9-4cb5-b647-77f7f5e8eb6b)
  * Enter the name
    
    ![Screenshot (44)](https://github.com/user-attachments/assets/e6752438-cc2e-43b1-85c3-b7d82733b044)
  * Show the details (if found)
    
    ![Screenshot (45)](https://github.com/user-attachments/assets/307cbc2b-7ff2-432a-bb1d-ad31f2b1a88c)
### 4. Edit Contact
  * Enter a person name that you want to edit
    
    ![Screenshot (46)](https://github.com/user-attachments/assets/79f68a2a-449a-4c6d-92a8-fb8901a9695c)
  * Choose which section you want to edit

    ![Screenshot (48)](https://github.com/user-attachments/assets/1ddb2da3-86ef-4ff7-bd6d-f47a4e569a21)

  * Edit contact successful
    
    ![Screenshot (50)](https://github.com/user-attachments/assets/7a807606-46f5-48b8-aa12-997236f8a013)
### 5. List of Contact
![Screenshot (51)](https://github.com/user-attachments/assets/b5fed2ab-1368-4f2b-a85b-8f2888def084)
### 6. Delete Contact
  * Enter a person name you want to delete

    ![Screenshot (52)](https://github.com/user-attachments/assets/dd0b3c9b-b46f-4940-b556-7ad45cf89807)
  * Ensure you want to delete the contact

    ![Screenshot (53)](https://github.com/user-attachments/assets/d37af84d-75ff-44bb-9af1-84fa8b1b363a)
  * Delete Successful

    ![Screenshot (54)](https://github.com/user-attachments/assets/d2156f90-e661-4a6c-822d-f0339f0fab9c)







