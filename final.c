#include <stdio.h>   // Standard input/output library (printf, scanf, getchar)
#include <string.h>  // String functions library (strcspn, strcmp, fgets)

// ANSI color codes stored as macros for easy reuse in printf statements
#define RESET   "\033[0m"        // Resets all color/style back to terminal default
#define BOLD    "\033[1m"        // Makes text bold/bright
#define RED     "\033[1;31m"     // Bold red — used for errors and warnings
#define GREEN   "\033[1;32m"     // Bold green — used for success messages
#define YELLOW  "\033[1;33m"     // Bold yellow — used for prompts and highlights
#define CYAN    "\033[1;36m"     // Bold cyan — used for headers and menus
#define WHITE   "\033[1;37m"     // Bold white — used for general important text
#define MAGENTA "\033[1;35m"     // Bold magenta — used for book details

// Define a struct (custom data type) named Book to group related book data
struct Book {
    int id;          // Stores the unique numeric ID of the book
    char name[50];   // Stores the book title (up to 49 characters + null terminator)
    char author[50]; // Stores the author's name (up to 49 characters)
    char topic[50];  // Stores the topic/genre (up to 49 characters)
    int status;      // Tracks availability: 0 = Available, 1 = Issued
};

struct Book b[100]; // Declare a global array of 100 Book structs to store all books
int count = 0;      // Global counter that tracks how many books have been added

// ─────────────────────────────────────────────
// UTILITY: clearBuffer
// Clears leftover characters from the input buffer
// after using scanf, to prevent input skipping bugs
// ─────────────────────────────────────────────
void clearBuffer() {
    int c;                                      // Temporary variable to hold each character
    while ((c = getchar()) != '\n' && c != EOF); // Read and discard chars until newline or end-of-file
}

// ─────────────────────────────────────────────
// UTILITY: printLine
// Prints a decorative horizontal divider line
// ─────────────────────────────────────────────
void printLine() {
    printf(CYAN "══════════════════════════════════════════════════\n" RESET); // Prints a styled separator
}

// ─────────────────────────────────────────────
// FUNCTION: addBook
// Prompts the user to enter book details and
// saves the new book into the b[] array
// ─────────────────────────────────────────────
void addBook() {
    printLine();                                           // Print top border
    printf(CYAN BOLD "          📚  ADD NEW BOOK\n" RESET); // Section header
    printLine();                                           // Print bottom border

    printf(YELLOW "Enter Book ID   : " RESET);            // Prompt user for the book ID
    scanf("%d", &b[count].id);                            // Read integer and store in current book's id field
                                                          // &b[count].id = address of the id field of the next slot

    clearBuffer();                                        // Clear newline left in buffer after scanf

    printf(YELLOW "Enter Book Name : " RESET);            // Prompt for book title
    fgets(b[count].name, 50, stdin);                      // Read up to 49 chars from keyboard into name field
    b[count].name[strcspn(b[count].name, "\n")] = 0;     // Remove the trailing newline fgets adds

    printf(YELLOW "Enter Author    : " RESET);            // Prompt for author name
    fgets(b[count].author, 50, stdin);                    // Read author name from keyboard
    b[count].author[strcspn(b[count].author, "\n")] = 0; // Strip trailing newline

    printf(YELLOW "Enter Topic     : " RESET);            // Prompt for topic/genre
    fgets(b[count].topic, 50, stdin);                     // Read topic from keyboard
    b[count].topic[strcspn(b[count].topic, "\n")] = 0;   // Strip trailing newline

    b[count].status = 0;  // Set status to 0 (Available) since it's a new book
    count++;              // Increment global counter so next book goes into the next array slot

    printf(GREEN "\n  ✅  Book added successfully!\n" RESET); // Confirm success to the user
    printLine();                                               // Print closing border
}

// ─────────────────────────────────────────────
// FUNCTION: displayBooks
// Loops through all books in b[] and prints
// each one in a formatted table
// ─────────────────────────────────────────────
void displayBooks() {
    printLine();
    printf(CYAN BOLD "          📖  ALL BOOKS IN LIBRARY\n" RESET); // Section header
    printLine();

    if (count == 0) {                                      // Check if no books have been added yet
        printf(RED "  No books available.\n" RESET);       // Inform user the library is empty
        printLine();
        return;                                            // Exit the function early — nothing to display
    }

    // Print table column headers with color styling
    printf(BOLD WHITE "  %-5s %-25s %-20s %-15s %-10s\n" RESET,
           "ID", "Name", "Author", "Topic", "Status");
    // %-5s means: left-align string in a field 5 characters wide (padded with spaces)
    printf(CYAN "  ─────────────────────────────────────────────────────────────────────────\n" RESET);

    for (int i = 0; i < count; i++) { // Loop from index 0 to count-1 (every stored book)

        // Choose color based on availability status
        const char *statusColor = (b[i].status == 0) ? GREEN : RED; // Green if available, red if issued
        const char *statusText  = (b[i].status == 0) ? "Available" : "Issued"; // Human-readable label

        printf(MAGENTA "  %-5d" RESET, b[i].id);            // Print book ID in magenta, padded to 5 chars
        printf(" %-25s", b[i].name);                         // Print name padded to 25 chars
        printf(" %-20s", b[i].author);                       // Print author padded to 20 chars
        printf(" %-15s", b[i].topic);                        // Print topic padded to 15 chars
        printf(" %s%-10s%s\n", statusColor, statusText, RESET); // Print colored status, then reset color
    }

    printLine(); // Closing border after table
}

// ─────────────────────────────────────────────
// FUNCTION: searchByID
// Asks for a book ID and searches b[] for a match
// ─────────────────────────────────────────────
void searchByID() {
    int id, found = 0;  // Declare id to search for; found flag starts at 0 (false)

    printLine();
    printf(CYAN BOLD "          🔍  SEARCH BY ID\n" RESET);
    printLine();

    printf(YELLOW "Enter Book ID: " RESET);  // Prompt user
    scanf("%d", &id);                         // Read the ID to search for

    for (int i = 0; i < count; i++) {         // Loop through every stored book
        if (b[i].id == id) {                  // Compare stored ID with the searched ID
            const char *statusText = (b[i].status == 0) ? "Available" : "Issued";
            const char *statusColor = (b[i].status == 0) ? GREEN : RED;

            printf(GREEN "\n  ✔  Book Found!\n" RESET);     // Confirm match found
            printf(WHITE "  Name   : " RESET "%s\n", b[i].name);     // Display name
            printf(WHITE "  Author : " RESET "%s\n", b[i].author);   // Display author
            printf(WHITE "  Topic  : " RESET "%s\n", b[i].topic);    // Display topic
            printf(WHITE "  Status : " RESET "%s%s%s\n", statusColor, statusText, RESET); // Colored status
            found = 1;  // Set flag to 1 (true) to indicate a match was found
        }
    }

    if (!found)  // If found is still 0 after the loop, no match was found
        printf(RED "\n  ✘  Book not found!\n" RESET);

    printLine();
}

// ─────────────────────────────────────────────
// FUNCTION: searchByTopic
// Searches b[] for all books matching a given topic
// ─────────────────────────────────────────────
void searchByTopic() {
    char topic[50]; // Local array to store the topic entered by the user
    int found = 0;  // Flag to track whether any matches were found

    printLine();
    printf(CYAN BOLD "          🔍  SEARCH BY TOPIC\n" RESET);
    printLine();

    clearBuffer();  // Clear input buffer before fgets to avoid skipping input

    printf(YELLOW "Enter Topic: " RESET);       // Prompt user
    fgets(topic, 50, stdin);                    // Read topic string from keyboard
    topic[strcspn(topic, "\n")] = 0;           // Remove trailing newline from input

    printf("\n"); // Add blank line before results for readability

    for (int i = 0; i < count; i++) {          // Loop through every stored book
        if (strcmp(b[i].topic, topic) == 0) {  // strcmp returns 0 if strings are equal (exact match)
            printf(MAGENTA "  [%d]" RESET " %s — %s\n",
                   b[i].id, b[i].name, b[i].author); // Print matching book details
            found = 1;  // At least one match found
        }
    }

    if (!found)
        printf(RED "  ✘  No books found under that topic.\n" RESET);

    printLine();
}

// ─────────────────────────────────────────────
// FUNCTION: issueBook
// Marks a book as Issued (status = 1) if it is
// currently Available (status = 0)
// ─────────────────────────────────────────────
void issueBook() {
    int id;  // Will hold the ID of the book to issue

    printLine();
    printf(CYAN BOLD "          📤  ISSUE BOOK\n" RESET);
    printLine();

    printf(YELLOW "Enter Book ID: " RESET);  // Prompt user
    scanf("%d", &id);                         // Read book ID

    for (int i = 0; i < count; i++) {         // Loop through all books
        if (b[i].id == id) {                  // If ID matches
            if (b[i].status == 0) {           // Check if book is currently available
                b[i].status = 1;              // Change status to Issued
                printf(GREEN "\n  ✅  Book issued successfully!\n" RESET);
            } else {
                printf(RED "\n  ⚠  Book is already issued!\n" RESET); // Cannot issue an already-issued book
            }
            printLine();
            return;  // Exit function after finding the book (no need to keep looping)
        }
    }

    printf(RED "\n  ✘  Book not found!\n" RESET); // ID didn't match any book
    printLine();
}

// ─────────────────────────────────────────────
// FUNCTION: returnBook
// Marks a book as Available (status = 0) if it
// was previously Issued (status = 1)
// ─────────────────────────────────────────────
void returnBook() {
    int id;  // Will hold the ID of the book being returned

    printLine();
    printf(CYAN BOLD "          📥  RETURN BOOK\n" RESET);
    printLine();

    printf(YELLOW "Enter Book ID: " RESET);  // Prompt user
    scanf("%d", &id);                         // Read book ID

    for (int i = 0; i < count; i++) {         // Loop through all books
        if (b[i].id == id) {                  // ID match found
            if (b[i].status == 1) {           // Only return if book is currently issued
                b[i].status = 0;              // Set status back to Available
                printf(GREEN "\n  ✅  Book returned successfully!\n" RESET);
            } else {
                printf(RED "\n  ⚠  This book was not issued!\n" RESET); // Cannot return an available book
            }
            printLine();
            return;  // Stop looping after finding the book
        }
    }

    printf(RED "\n  ✘  Book not found!\n" RESET);
    printLine();
}

// ─────────────────────────────────────────────
// FUNCTION: deleteBook
// Removes a book from b[] by shifting all
// subsequent elements one position left
// ─────────────────────────────────────────────
void deleteBook() {
    int id;  // ID of the book to delete

    printLine();
    printf(CYAN BOLD "          🗑️   DELETE BOOK\n" RESET);
    printLine();

    printf(YELLOW "Enter Book ID: " RESET);  // Prompt user
    scanf("%d", &id);                         // Read book ID

    for (int i = 0; i < count; i++) {           // Loop through books
        if (b[i].id == id) {                    // Match found at index i
            for (int j = i; j < count - 1; j++) { // Shift every book after index i one step left
                b[j] = b[j + 1];               // Copy the next book into the current slot (overwrites deleted book)
            }
            count--;  // Decrease count since one book was removed
            printf(GREEN "\n  ✅  Book deleted successfully!\n" RESET);
            printLine();
            return;  // Stop after deletion
        }
    }

    printf(RED "\n  ✘  Book not found!\n" RESET);
    printLine();
}

// ─────────────────────────────────────────────
// FUNCTION: main
// Entry point of the program. Shows a menu in a
// loop and calls the appropriate function based
// on the user's numeric choice
// ─────────────────────────────────────────────
int main() {
    int choice;  // Stores the menu option selected by the user

    do {  // do-while loop: runs at least once, keeps running until user chooses 8 (Exit)

        // Print the main menu with colors and borders
        printf("\n");
        printLine();
        printf(CYAN BOLD "        📚  LIBRARY MANAGEMENT SYSTEM\n" RESET);
        printLine();
        printf(WHITE "  1." RESET "  ➕  Add Book\n");       // Option 1
        printf(WHITE "  2." RESET "  📋  Display Books\n");  // Option 2
        printf(WHITE "  3." RESET "  🔍  Search by ID\n");   // Option 3
        printf(WHITE "  4." RESET "  🔎  Search by Topic\n");// Option 4
        printf(WHITE "  5." RESET "  📤  Issue Book\n");     // Option 5
        printf(WHITE "  6." RESET "  📥  Return Book\n");    // Option 6
        printf(WHITE "  7." RESET "  🗑️   Delete Book\n");    // Option 7
        printf(WHITE "  8." RESET "  🚪  Exit\n");           // Option 8
        printLine();
        printf(YELLOW "  Enter your choice: " RESET);        // Prompt for choice
        scanf("%d", &choice);                                 // Read integer choice from user

        // switch checks value of choice and jumps to matching case
        switch (choice) {
            case 1: addBook();      break; // Call addBook() then break out of switch
            case 2: displayBooks(); break; // Call displayBooks()
            case 3: searchByID();   break; // Call searchByID()
            case 4: searchByTopic();break; // Call searchByTopic()
            case 5: issueBook();    break; // Call issueBook()
            case 6: returnBook();   break; // Call returnBook()
            case 7: deleteBook();   break; // Call deleteBook()
            case 8:
                printLine();
                printf(CYAN "  👋  Goodbye! Thank you for using the Library System.\n" RESET);
                printLine();
                break; // Print exit message and break
            default:
                printf(RED "  ✘  Invalid choice! Please enter 1–8.\n" RESET); // Any other number is invalid
        }

    } while (choice != 8); // Keep looping until user picks 8

    return 0; // Return 0 to the OS, indicating the program ended successfully
}