//#include <iostream>
//#include <string>
//#include <iomanip>
//#include <windows.h>
//#include <stdexcept>
//#include <string.h>
//using namespace std;
//
//// Constants
//const int MAX_USERS = 100;
//const int MAX_FILES = 100;
//const int MAX_RECENT = 10;
//const int MAX_FOLDER_DEPTH = 10;
//const int MAX_VERSIONS = 10;
//const int MAX_GROUPS = 10;
//const int HASH_TABLE_SIZE = 101;
//
//// Color definitions for Windows
//#define COLOR_RESET 7
//#define COLOR_RED 12
//#define COLOR_GREEN 10
//#define COLOR_YELLOW 14
//#define COLOR_BLUE 9
//#define COLOR_MAGENTA 13
//#define COLOR_CYAN 11
//
//// Helper function to set console color
//void setColor(int color) {
//    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
//}
//
//// Data Structures
//struct FileVersionNode {
//    string versionContent;
//    FileVersionNode* next;
//    FileVersionNode(const string& content) : versionContent(content), next(nullptr) {}
//};
//
//struct FileVersionList {
//    FileVersionNode* head;
//    int count;
//    FileVersionList() : head(nullptr), count(0) {}
//};
//
//struct GraphNode {
//    string name;
//    bool isUser;
//    GraphNode* next;
//    GraphNode(const string& n, bool user) : name(n), isUser(user), next(nullptr) {}
//};
//
//struct GraphEdge {
//    string source;
//    string destination;
//    GraphEdge* next;
//    GraphEdge(const string& src, const string& dest) : source(src), destination(dest), next(nullptr) {}
//};
//
//struct User {
//    string username;
//    string password;
//    string accessLevel;
//    string group;
//    GraphNode* graphNode = nullptr; // Initialized
//};
//
//struct File {
//    string name;
//    string path;
//    string content;
//    bool compressed = false; // Initialized
//    string compressionType;
//    string hash;
//    FileVersionList versionList;
//    GraphNode* graphNode = nullptr; // Initialized
//};
//
//struct Folder {
//    string name;
//    string path;
//    // If your Folder struct has a parent member, uncomment the following:
//    // string parent = "";
//};
//
//struct RecycleBinEntry {
//    string path;
//    File file;
//};
//
//struct SharedFile {
//    string filePath;
//    string sharedWith;
//};
//
//struct UserGroup {
//    string groupName;
//    string permissions;
//    string members[MAX_USERS];
//    int memberCount = 0; // Initialized
//};
//
//// Global variables
//User users[MAX_USERS];
//int userCount = 0;
//File files[MAX_FILES];
//int fileCount = 0;
//RecycleBinEntry recycleBin[MAX_FILES];
//int recycleCount = 0;
//
//string recentFilesQueue[MAX_RECENT];
//int recentFront = 0, recentRear = -1, recentCount = 0;
//
//Folder folders[MAX_FILES];
//int folderCount = 0;
//
//SharedFile sharedFiles[MAX_FILES];
//int sharedCount = 0;
//
//UserGroup userGroups[MAX_GROUPS];
//int groupCount = 0;
//
//GraphNode* graphNodes = nullptr;
//GraphEdge* graphEdges = nullptr;
//
//string currentUser = "";
//string currentPath = "/";
//
//// Helper Functions
//void intToString(int num, char* str, int maxSize) {
//    if (num < 0) {
//        str[0] = '0';
//        str[1] = '\0';
//        return;
//    }
//    if (num == 0) {
//        str[0] = '0';
//        str[1] = '\0';
//        return;
//    }
//    char temp[20];
//    int i = 0;
//    while (num > 0 && i < 19) {
//        temp[i++] = '0' + (num % 10);
//        num /= 10;
//    }
//    int j = 0;
//    while (i > 0 && j < maxSize - 1) {
//        str[j++] = temp[--i];
//    }
//    str[j] = '\0';
//}
//
//void freeVersionList(FileVersionList& list) {
//    FileVersionNode* current = list.head;
//    while (current != nullptr) {
//        FileVersionNode* temp = current;
//        current = current->next;
//        delete temp;
//    }
//    list.head = nullptr;
//    list.count = 0;
//}
//
//unsigned int hashFile(const string& content) {
//    unsigned int hash = 0;
//    for (char c : content) {
//        hash = (hash * 31) + c;
//    }
//    return hash % HASH_TABLE_SIZE;
//}
//
//void resolveHashConflict(int index, const string& content) {
//    int originalIndex = index;
//    do {
//        if (files[index].content.empty()) {
//            return;
//        }
//        index = (index + 1) % HASH_TABLE_SIZE;
//    } while (index != originalIndex);
//    cout << "Hash table full. Cannot resolve conflict.\n";
//}
//
//void pushToStack(File& file) {
//    if (recycleCount >= MAX_FILES) {
//        cout << "Recycle bin is full. Cannot delete file.\n";
//        return;
//    }
//    recycleBin[recycleCount].path = file.path;
//    recycleBin[recycleCount].file = file;
//    recycleCount++;
//}
//
//File* popFromStack() {
//    if (recycleCount == 0) {
//        cout << "Recycle bin is empty.\n";
//        return nullptr;
//    }
//    return &recycleBin[--recycleCount].file;
//}
//
//void enqueueFile(const string& filePath) {
//    if (recentCount < MAX_RECENT) {
//        recentRear = (recentRear + 1) % MAX_RECENT;
//        recentFilesQueue[recentRear] = filePath;
//        recentCount++;
//    }
//    else {
//        recentFront = (recentFront + 1) % MAX_RECENT;
//        recentRear = (recentRear + 1) % MAX_RECENT;
//        recentFilesQueue[recentRear] = filePath;
//    }
//}
//
//string dequeueFile() {
//    if (recentCount == 0) {
//        return "";
//    }
//    string filePath = recentFilesQueue[recentFront];
//    recentFront = (recentFront + 1) % MAX_RECENT;
//    recentCount--;
//    return filePath;
//}
//
//void addNode(const string& name, bool isUser) {
//    GraphNode* newNode = new GraphNode(name, isUser);
//    newNode->next = graphNodes;
//    graphNodes = newNode;
//
//    if (isUser) {
//        for (int i = 0; i < userCount; i++) {
//            if (users[i].username == name) {
//                users[i].graphNode = newNode;
//                break;
//            }
//        }
//    }
//    else {
//        for (int i = 0; i < fileCount; i++) {
//            if (files[i].path == name) {
//                files[i].graphNode = newNode;
//                break;
//            }
//        }
//    }
//}
//
//void addEdge(const string& source, const string& destination) {
//    GraphEdge* newEdge = new GraphEdge(source, destination);
//    newEdge->next = graphEdges;
//    graphEdges = newEdge;
//}
//
//void insertToLinkedList(FileVersionList& list, const string& content) {
//    FileVersionNode* newNode = new FileVersionNode(content);
//    newNode->next = list.head;
//    list.head = newNode;
//    list.count++;
//}
//
//void traverseLinkedList(const FileVersionList& list) {
//    FileVersionNode* current = list.head;
//    int versionNum = list.count;
//    while (current != nullptr) {
//        cout << "Version " << versionNum-- << ":\n";
//        cout << current->versionContent << "\n\n";
//        current = current->next;
//    }
//}
//
//// Helper function to check if folder2 is a direct child of folder1
//bool isDirectChild(const Folder& folder1, const Folder& folder2) {
//    if (folder2.path == folder1.path) return false;
//    if (folder2.path.find(folder1.path) != 0) return false;
//
//    string relativePath = folder2.path.substr(folder1.path.length());
//    size_t pos = relativePath.find('/', 1);
//    return pos == string::npos || pos == relativePath.length() - 1;
//}
//
//// Helper function to get folder depth based on path
//int getFolderDepth(const string& path) {
//    if (path == "/") return 0;
//    int depth = 0;
//    for (size_t i = 0; i < path.length(); i++) {
//        if (path[i] == '/') depth++;
//    }
//    return depth - 1;
//}
//
//// Helper function to find folder index by path
//int findFolderIndex(const string& path) {
//    for (int i = 0; i < folderCount; i++) {
//        if (folders[i].path == path) return i;
//    }
//    return -1;
//}
//
//
//
//
//// Tree traversal function definition
//void treeTraversal(bool dfs) {
//    if (folderCount == 0) {
//        cout << "No folders to traverse.\n";
//        return;
//    }
//
//    cout << "File System Tree (" << (dfs ? "DFS" : "BFS") << " Traversal):\n";
//
//    // Temporary stack for DFS using File objects
//    File tempStack[MAX_FILES];
//    int tempStackTop = -1;
//
//    // Initialize tempStack elements
//    for (int i = 0; i < MAX_FILES; i++) {
//        tempStack[i].compressed = false;
//        tempStack[i].graphNode = nullptr;
//        tempStack[i].versionList = FileVersionList();
//    }
//
//    // Preserve original recycleBin state
//    RecycleBinEntry originalRecycleBin[MAX_FILES];
//    int originalRecycleCount = recycleCount;
//    for (int i = 0; i < recycleCount; i++) {
//        originalRecycleBin[i] = recycleBin[i];
//    }
//    recycleCount = 0;
//
//    // Preserve original recent files queue state
//    string originalQueue[MAX_RECENT];
//    int originalFront = recentFront, originalRear = recentRear, originalCount = recentCount;
//    for (int i = 0; i < MAX_RECENT; i++) {
//        originalQueue[i] = recentFilesQueue[i];
//    }
//    recentFront = 0;
//    recentRear = -1;
//    recentCount = 0;
//
//    // Track visited folders
//    bool visited[MAX_FILES] = { false };
//
//    // Find root folder
//    int rootIndex = -1;
//    for (int i = 0; i < folderCount; i++) {
//        if (folders[i].path == "/") {
//            rootIndex = i;
//            break;
//        }
//    }
//    if (rootIndex == -1) {
//        cout << "Root folder not found.\n";
//        return;
//    }
//
//    // Initialize traversal
//    if (dfs) {
//        tempStack[++tempStackTop].path = folders[rootIndex].path;
//        pushToStack(tempStack[tempStackTop]);
//    }
//    else {
//        enqueueFile(folders[rootIndex].path);
//    }
//    visited[rootIndex] = true;
//
//    while ((dfs && recycleCount > 0) || (!dfs && recentCount > 0)) {
//        int currentIndex;
//        if (dfs) {
//            File* popped = popFromStack();
//            if (popped == nullptr) break;
//            currentIndex = findFolderIndex(popped->path);
//        }
//        else {
//            string path = dequeueFile();
//            currentIndex = findFolderIndex(path);
//        }
//
//        if (currentIndex < 0 || currentIndex >= folderCount) continue;
//
//        // Print folder with indentation
//        int depth = getFolderDepth(folders[currentIndex].path);
//        for (int i = 0; i < depth * 2; i++) cout << " ";
//        string name = (folders[currentIndex].path == "/") ? "/" : folders[currentIndex].name;
//        cout << "L-- " << name << "\n";
//
//        // Collect children
//        int children[MAX_FILES];
//        int childCount = 0;
//        for (int i = 0; i < folderCount; i++) {
//            if (!visited[i] && isDirectChild(folders[currentIndex], folders[i])) {
//                children[childCount++] = i;
//            }
//        }
//
//        // Sort children alphabetically (bubble sort)
//        for (int i = 0; i < childCount - 1; i++) {
//            for (int j = 0; j < childCount - i - 1; j++) {
//                if (folders[children[j]].name > folders[children[j + 1]].name) {
//                    int temp = children[j];
//                    children[j] = children[j + 1];
//                    children[j + 1] = temp;
//                }
//            }
//        }
//
//        // Add children to stack (reverse for DFS) or queue
//        if (dfs) {
//            for (int i = childCount - 1; i >= 0; i--) {
//                if (tempStackTop + 1 < MAX_FILES) {
//                    tempStack[++tempStackTop].path = folders[children[i]].path;
//                    pushToStack(tempStack[tempStackTop]);
//                    visited[children[i]] = true;
//                }
//            }
//        }
//        else {
//            for (int i = 0; i < childCount; i++) {
//                enqueueFile(folders[children[i]].path);
//                visited[children[i]] = true;
//            }
//        }
//    }
//
//    // Restore recycleBin state
//    for (int i = 0; i < originalRecycleCount; i++) {
//        recycleBin[i] = originalRecycleBin[i];
//    }
//    recycleCount = originalRecycleCount;
//
//    // Restore recent files queue state
//    recentFront = originalFront;
//    recentRear = originalRear;
//    recentCount = originalCount;
//    for (int i = 0; i < MAX_RECENT; i++) {
//        recentFilesQueue[i] = originalQueue[i];
//    }
//}
//
//// UI Functions
//void printHeader(const string& title) {
//    setColor(COLOR_CYAN);
//    cout << "\n+--------------------------------------------+\n";
//    cout << "|";
//    size_t spaces = (44 - title.length()) / 2;
//    cout << string(spaces, ' ') << title << string(44 - title.length() - spaces, ' ');
//    cout << "|\n";
//    cout << "+--------------------------------------------+\n";
//    setColor(COLOR_RESET);
//}
//
//void printMenuOption(int num, const string& text) {
//    setColor(COLOR_YELLOW);
//    cout << " " << num << ". ";
//    setColor(COLOR_RESET);
//    cout << text << "\n";
//}
//
//void printCurrentUserInfo() {
//    if (!currentUser.empty()) {
//        setColor(COLOR_GREEN);
//        cout << "\nLogged in as: " << currentUser;
//        cout << " | Current path: " << currentPath << "\n";
//        setColor(COLOR_RESET);
//    }
//    else {
//        setColor(COLOR_RED);
//        cout << "\nNot logged in\n";
//        setColor(COLOR_RESET);
//    }
//}
//
//void printSuccess(const string& message) {
//    setColor(COLOR_GREEN);
//    cout << "* " << message << "\n";
//    setColor(COLOR_RESET);
//}
//
//void printError(const string& message) {
//    setColor(COLOR_RED);
//    cout << "X " << message << "\n";
//    setColor(COLOR_RESET);
//}
//
//void printWarning(const string& message) {
//    setColor(COLOR_YELLOW);
//    cout << "! " << message << "\n";
//    setColor(COLOR_RESET);
//}
//
//int getValidChoice(int maxChoice) {
//    string input;
//    int choice;
//    while (true) {
//        setColor(COLOR_YELLOW);
//        cout << "Enter your choice (0-" << maxChoice << "): ";
//        setColor(COLOR_RESET);
//        cin.clear();
//        cin.sync();
//        getline(cin, input);
//
//        // Trim whitespace
//        input.erase(0, input.find_first_not_of(" \t"));
//        input.erase(input.find_last_not_of(" \t") + 1);
//
//        // Check for empty input
//        if (input.empty()) {
//            printError("Input cannot be empty. Please enter a number.");
//            continue;
//        }
//
//        // Check if input is numeric
//        bool isNumeric = true;
//        for (char c : input) {
//            if (!isdigit(c)) {
//                isNumeric = false;
//                break;
//            }
//        }
//        if (!isNumeric) {
//            printError("Please enter a valid number (no letters or special characters).");
//            continue;
//        }
//
//        // Convert to integer and validate range
//        try {
//            choice = stoi(input);
//            if (choice >= 0 && choice <= maxChoice) {
//                return choice;
//            }
//            else {
//                printError("Choice must be between 0 and " + to_string(maxChoice) + ".");
//            }
//        }
//        catch (const invalid_argument&) {
//            printError("Invalid input. Please enter a valid number.");
//        }
//        catch (const out_of_range&) {
//            printError("Number too large. Please enter a valid choice.");
//        }
//    }
//}
//
//// File System Functions
//void initializeFileSystem() {
//    folderCount = 1;
//    folders[0].name = "/";
//    folders[0].path = "/";
//    currentPath = "/";
//}
//
//void registerUser() {
//    if (userCount >= MAX_USERS) {
//        printError("Maximum number of users reached.");
//        return;
//    }
//    try {
//        string username, password, accessLevel;
//        cout << "Enter username: ";
//        getline(cin, username);
//
//        bool isEmpty = true;
//        for (size_t i = 0; i < username.length(); i++) {
//            isEmpty = false;
//            break;
//        }
//        if (isEmpty) {
//            throw invalid_argument("Username cannot be empty.");
//        }
//
//        cout << "Enter password (minimum 6 characters): ";
//        getline(cin, password);
//        if (password.length() < 6) {
//            throw invalid_argument("Password must be at least 6 characters long.");
//        }
//
//        cout << "Enter access level (admin/user): ";
//        getline(cin, accessLevel);
//        if (accessLevel != "admin" && accessLevel != "user") {
//            throw invalid_argument("Access level must be 'admin' or 'user'.");
//        }
//
//        users[userCount].username = username;
//        users[userCount].password = password;
//        users[userCount].accessLevel = accessLevel;
//        users[userCount].graphNode = nullptr;
//
//        addNode(username, true);
//        userCount++;
//        printSuccess("User registered successfully!");
//    }
//    catch (const invalid_argument& e) {
//        printError(e.what());
//    }
//    catch (const exception& e) {
//        printError("An error occurred: " + string(e.what()));
//    }
//}
//
//void loginUser() {
//    string username, password;
//    cout << "Enter username: ";
//    getline(cin, username);
//    cout << "Enter password: ";
//    getline(cin, password);
//
//    for (int i = 0; i < userCount; i++) {
//        if (users[i].username == username && users[i].password == password) {
//            currentUser = username;
//            printSuccess("Login successful!");
//            return;
//        }
//    }
//    printError("Invalid credentials!");
//}
//
//void logoutUser() {
//    currentUser = "";
//    printSuccess("Logged out successfully!");
//}
//
//void resetPassword() {
//    try {
//        string username, oldPassword, newPassword;
//        cout << "Enter username: ";
//        getline(cin, username);
//        cout << "Enter old password: ";
//        getline(cin, oldPassword);
//
//        for (int i = 0; i < userCount; i++) {
//            if (users[i].username == username && users[i].password == oldPassword) {
//                cout << "Enter new password (minimum 6 characters): ";
//                getline(cin, newPassword);
//
//                if (newPassword.length() < 6) {
//                    throw invalid_argument("New password must be at least 6 characters long.");
//                }
//
//                users[i].password = newPassword;
//                printSuccess("Password reset successfully!");
//                return;
//            }
//        }
//        printError("Invalid credentials!");
//    }
//    catch (const invalid_argument& e) {
//        printError(e.what());
//    }
//    catch (const exception& e) {
//        printError("An error occurred: " + string(e.what()));
//    }
//}
//
//void createFolder() {
//    if (folderCount >= MAX_FILES) {
//        printError("Maximum number of folders reached.");
//        return;
//    }
//
//    string folderName;
//    cout << "Enter folder name: ";
//    getline(cin, folderName);
//
//    if (folderName.empty()) {
//        printError("Folder name cannot be empty.");
//        return;
//    }
//
//    string fullPath = currentPath + folderName + "/";
//    for (int i = 0; i < folderCount; i++) {
//        if (folders[i].path == fullPath) {
//            printError("Folder already exists.");
//            return;
//        }
//    }
//
//    folders[folderCount].name = folderName;
//    folders[folderCount].path = fullPath;
//    folderCount++;
//    printSuccess("Folder created.");
//}
//
//void deleteFolder() {
//    string folderName;
//    cout << "Enter folder name to delete: ";
//    getline(cin, folderName);
//
//    string fullPath = currentPath + folderName + "/";
//    for (int i = 0; i < folderCount; i++) {
//        if (folders[i].path == fullPath) {
//            bool hasFiles = false;
//            for (int j = 0; j < fileCount; j++) {
//                if (files[j].path.find(folders[i].path) == 0) {
//                    hasFiles = true;
//                    break;
//                }
//            }
//
//            if (hasFiles) {
//                printError("Folder is not empty. Cannot delete.");
//                return;
//            }
//
//            for (int j = i; j < folderCount - 1; j++) {
//                folders[j] = folders[j + 1];
//            }
//            folderCount--;
//            printSuccess("Folder deleted.");
//            return;
//        }
//    }
//    printError("Folder not found.");
//}
//
//void navigateFolder() {
//    string folderName;
//    cout << "Enter folder name to navigate (or '..' to go up): ";
//    getline(cin, folderName);
//
//    if (folderName == "..") {
//        if (currentPath == "/") {
//            printWarning("Already at root directory.");
//            return;
//        }
//
//        size_t pos = currentPath.substr(0, currentPath.size() - 1).rfind('/');
//        currentPath = currentPath.substr(0, pos + 1);
//        printSuccess("Navigated to " + currentPath);
//        return;
//    }
//
//    string fullPath = currentPath + folderName + "/";
//    for (int i = 0; i < folderCount; i++) {
//        if (folders[i].path == fullPath) {
//            currentPath = fullPath;
//            printSuccess("Navigated to " + currentPath);
//            return;
//        }
//    }
//    printError("Folder not found.");
//}
//
//void createFile() {
//    if (fileCount >= MAX_FILES) {
//        printError("Maximum number of files reached.");
//        return;
//    }
//
//    string fileName;
//    cout << "Enter file name: ";
//    getline(cin, fileName);
//
//    if (fileName.empty()) {
//        printError("File name cannot be empty.");
//        return;
//    }
//
//    string fullPath = currentPath + fileName;
//    for (int i = 0; i < fileCount; i++) {
//        if (files[i].path == fullPath) {
//            printError("File already exists.");
//            return;
//        }
//    }
//
//    files[fileCount].name = fileName;
//    files[fileCount].path = fullPath;
//    cout << "Enter file content: ";
//    getline(cin, files[fileCount].content);
//    files[fileCount].compressed = false;
//    files[fileCount].compressionType = "";
//    files[fileCount].graphNode = nullptr;
//    files[fileCount].versionList = FileVersionList();
//    char hashStr[20];
//    intToString(hashFile(files[fileCount].content), hashStr, 20);
//    files[fileCount].hash = hashStr;
//    insertToLinkedList(files[fileCount].versionList, files[fileCount].content);
//    addNode(files[fileCount].path, false);
//    enqueueFile(files[fileCount].path);
//
//    fileCount++;
//    printSuccess("File created.");
//}
//
//void readFile() {
//    string fileName;
//    cout << "Enter file name: ";
//    getline(cin, fileName);
//
//    string fullPath = currentPath + fileName;
//    for (int i = 0; i < fileCount; i++) {
//        if (files[i].path == fullPath) {
//            setColor(COLOR_BLUE);
//            cout << "\n=========== FILE CONTENT ==========\n";
//            setColor(COLOR_RESET);
//            cout << (files[i].compressed ? "[COMPRESSED] " : "") << files[i].content << "\n";
//            setColor(COLOR_BLUE);
//            cout << "===================================\n";
//            setColor(COLOR_RESET);
//            enqueueFile(files[i].path);
//            return;
//        }
//    }
//    printError("File not found.");
//}
//
//void updateFile() {
//    string fileName;
//    cout << "Enter file name to update: ";
//    getline(cin, fileName);
//
//    string fullPath = currentPath + fileName;
//    for (int i = 0; i < fileCount; i++) {
//        if (files[i].path == fullPath) {
//            if (files[i].compressed) {
//                printError("Cannot update compressed file. Please decompress first.");
//                return;
//            }
//
//            setColor(COLOR_BLUE);
//            cout << "\n======== CURRENT CONTENT ========\n";
//            setColor(COLOR_RESET);
//            cout << files[i].content << "\n";
//            setColor(COLOR_BLUE);
//            cout << "=================================\n";
//            setColor(COLOR_RESET);
//
//            cout << "Enter new content: ";
//            getline(cin, files[i].content);
//
//            insertToLinkedList(files[i].versionList, files[i].content);
//            char hashStr[20];
//            intToString(hashFile(files[i].content), hashStr, 20);
//            files[i].hash = hashStr;
//
//            printSuccess("File updated.");
//            return;
//        }
//    }
//    printError("File not found.");
//}
//
//void deleteFile() {
//    string fileName;
//    cout << "Enter file name to delete: ";
//    getline(cin, fileName);
//
//    string fullPath = currentPath + fileName;
//    for (int i = 0; i < fileCount; i++) {
//        if (files[i].path == fullPath) {
//            pushToStack(files[i]);
//
//            for (int j = i; j < fileCount - 1; j++) {
//                files[j] = files[j + 1];
//            }
//            fileCount--;
//
//            printSuccess("File moved to recycle bin.");
//            return;
//        }
//    }
//    printError("File not found.");
//}
//
//void viewRecycleBin() {
//    setColor(COLOR_MAGENTA);
//    cout << "\n=========== RECYCLE BIN ==========\n";
//    setColor(COLOR_RESET);
//
//    if (recycleCount == 0) {
//        cout << "Recycle bin is empty.\n";
//    }
//    else {
//        for (int i = 0; i < recycleCount; i++) {
//            cout << i + 1 << ". " << recycleBin[i].path << "\n";
//        }
//    }
//
//    setColor(COLOR_MAGENTA);
//    cout << "==================================\n";
//    setColor(COLOR_RESET);
//}
//
//void restoreFile() {
//    if (recycleCount == 0) {
//        printError("Recycle bin is empty.");
//        return;
//    }
//
//    viewRecycleBin();
//    int fileNum = getValidChoice(recycleCount);
//
//    if (fileNum < 1 || fileNum > recycleCount) {
//        printError("Invalid file number.");
//        return;
//    }
//
//    if (fileCount >= MAX_FILES) {
//        printError("Cannot restore - maximum files reached.");
//        return;
//    }
//
//    files[fileCount++] = recycleBin[fileNum - 1].file;
//
//    for (int i = fileNum - 1; i < recycleCount - 1; i++) {
//        recycleBin[i] = recycleBin[i + 1];
//    }
//    recycleCount--;
//
//    printSuccess("File restored successfully.");
//}
//
//void emptyRecycleBin() {
//    for (int i = 0; i < recycleCount; i++) {
//        freeVersionList(recycleBin[i].file.versionList);
//        if (recycleBin[i].file.graphNode != nullptr) {
//            delete recycleBin[i].file.graphNode;
//            recycleBin[i].file.graphNode = nullptr;
//        }
//    }
//    recycleCount = 0;
//    printSuccess("Recycle bin emptied.");
//}
//
//void viewRecentFiles() {
//    setColor(COLOR_BLUE);
//    cout << "\n=========== RECENT FILES =========\n";
//    setColor(COLOR_RESET);
//
//    if (recentCount == 0) {
//        cout << "No recent files.\n";
//    }
//    else {
//        int index = recentFront;
//        for (int i = 0; i < recentCount; i++) {
//            cout << i + 1 << ". " << recentFilesQueue[index] << "\n";
//            index = (index + 1) % MAX_RECENT;
//        }
//    }
//
//    setColor(COLOR_BLUE);
//    cout << "==================================\n";
//    setColor(COLOR_RESET);
//}
//
//void searchFiles() {
//    string searchTerm;
//    cout << "Enter search term: ";
//    getline(cin, searchTerm);
//
//    setColor(COLOR_CYAN);
//    cout << "\n======== SEARCH RESULTS ========\n";
//    setColor(COLOR_RESET);
//
//    bool found = false;
//    for (int i = 0; i < fileCount; i++) {
//        if (files[i].name.find(searchTerm) != string::npos) {
//            cout << "- " << files[i].path << "\n";
//            found = true;
//        }
//    }
//
//    if (!found) {
//        cout << "No files found matching '" << searchTerm << "'\n";
//    }
//
//    setColor(COLOR_CYAN);
//    cout << "================================\n";
//    setColor(COLOR_RESET);
//}
//
//void shareFile() {
//    string fileName, recipient;
//    cout << "Enter file name to share: ";
//    getline(cin, fileName);
//    cout << "Enter recipient username: ";
//    getline(cin, recipient);
//
//    string fullPath = currentPath + fileName;
//
//    bool fileFound = false, userFound = false;
//    for (int i = 0; i < fileCount; i++) {
//        if (files[i].path == fullPath) {
//            fileFound = true;
//            break;
//        }
//    }
//    for (int i = 0; i < userCount; i++) {
//        if (users[i].username == recipient) {
//            userFound = true;
//            break;
//        }
//    }
//
//    if (!fileFound || !userFound) {
//        printError("File or user not found.");
//        return;
//    }
//
//    addEdge(recipient, fullPath);
//
//    if (sharedCount < MAX_FILES) {
//        sharedFiles[sharedCount].filePath = fullPath;
//        sharedFiles[sharedCount].sharedWith = recipient;
//        sharedCount++;
//        printSuccess("File shared successfully with " + recipient);
//    }
//    else {
//        printError("Maximum shared files reached.");
//    }
//}
//
//void revokeAccess() {
//    string fileName, recipient;
//    cout << "Enter file name: ";
//    getline(cin, fileName);
//    cout << "Enter recipient username: ";
//    getline(cin, recipient);
//
//    string fullPath = currentPath + fileName;
//
//    for (int i = 0; i < sharedCount; i++) {
//        if (sharedFiles[i].filePath == fullPath && sharedFiles[i].sharedWith == recipient) {
//            for (int j = i; j < sharedCount - 1; j++) {
//                sharedFiles[j] = sharedFiles[j + 1];
//            }
//            sharedCount--;
//            printSuccess("Access revoked successfully.");
//            return;
//        }
//    }
//
//    printError("No sharing record found for this file and user.");
//}
//
//void viewVersionHistory() {
//    string fileName;
//    cout << "Enter file name: ";
//    getline(cin, fileName);
//
//    string fullPath = currentPath + fileName;
//    for (int i = 0; i < fileCount; i++) {
//        if (files[i].path == fullPath) {
//            setColor(COLOR_MAGENTA);
//            cout << "\n======== VERSION HISTORY ========\n";
//            setColor(COLOR_RESET);
//            cout << "File: " << fileName << "\n\n";
//            traverseLinkedList(files[i].versionList);
//            setColor(COLOR_MAGENTA);
//            cout << "=================================\n";
//            setColor(COLOR_RESET);
//            return;
//        }
//    }
//
//    printError("File not found.");
//}
//
//void rollbackVersion() {
//    string fileName;
//    cout << "Enter file name: ";
//    getline(cin, fileName);
//
//    string fullPath = currentPath + fileName;
//    for (int i = 0; i < fileCount; i++) {
//        if (files[i].path == fullPath) {
//            setColor(COLOR_MAGENTA);
//            cout << "\n======== VERSION HISTORY ========\n";
//            setColor(COLOR_RESET);
//            cout << "File: " << fileName << "\n\n";
//            traverseLinkedList(files[i].versionList);
//            setColor(COLOR_MAGENTA);
//            cout << "=================================\n";
//            setColor(COLOR_RESET);
//
//            cout << "Enter version number to rollback to: ";
//            int versionNum = getValidChoice(files[i].versionList.count);
//
//            FileVersionNode* current = files[i].versionList.head;
//            int currentVersion = files[i].versionList.count;
//
//            while (current != nullptr && currentVersion > versionNum) {
//                current = current->next;
//                currentVersion--;
//            }
//
//            if (current != nullptr) {
//                files[i].content = current->versionContent;
//                char versionStr[20];
//                intToString(versionNum, versionStr, 20);
//                printSuccess("Rolled back to version " + string(versionStr));
//            }
//            else {
//                printError("Invalid version number.");
//            }
//            return;
//        }
//    }
//
//    printError("File not found.");
//}
//
//void setPermissions() {
//    string fileName, permissions;
//    cout << "Enter file name: ";
//    getline(cin, fileName);
//    cout << "Enter permissions (e.g., read, write, read/write): ";
//    getline(cin, permissions);
//
//    string fullPath = currentPath + fileName;
//    for (int i = 0; i < fileCount; i++) {
//        if (files[i].path == fullPath) {
//            printSuccess("Permissions set to '" + permissions + "' for " + fileName);
//            return;
//        }
//    }
//    printError("File not found.");
//}
//
//void createUserGroup() {
//    if (groupCount >= MAX_GROUPS) {
//        printError("Maximum number of groups reached.");
//        return;
//    }
//
//    string groupName, permissions;
//    cout << "Enter group name: ";
//    getline(cin, groupName);
//    cout << "Enter group permissions: ";
//    getline(cin, permissions);
//
//    userGroups[groupCount].groupName = groupName;
//    userGroups[groupCount].permissions = permissions;
//    userGroups[groupCount].memberCount = 0;
//    groupCount++;
//    printSuccess("Group created successfully.");
//}
//
//void addUserToGroup() {
//    string username, groupName;
//    cout << "Enter username: ";
//    getline(cin, username);
//    cout << "Enter group name: ";
//    getline(cin, groupName);
//
//    int userIndex = -1;
//    for (int i = 0; i < userCount; i++) {
//        if (users[i].username == username) {
//            userIndex = i;
//            break;
//        }
//    }
//
//    if (userIndex == -1) {
//        printError("User not found.");
//        return;
//    }
//
//    int groupIndex = -1;
//    for (int i = 0; i < groupCount; i++) {
//        if (userGroups[i].groupName == groupName) {
//            groupIndex = i;
//            break;
//        }
//    }
//
//    if (groupIndex == -1) {
//        printError("Group not found.");
//        return;
//    }
//
//    for (int i = 0; i < userGroups[groupIndex].memberCount; i++) {
//        if (userGroups[groupIndex].members[i] == username) {
//            printWarning("User is already in this group.");
//            return;
//        }
//    }
//
//    if (userGroups[groupIndex].memberCount < MAX_USERS) {
//        userGroups[groupIndex].members[userGroups[groupIndex].memberCount++] = username;
//        users[userIndex].group = groupName;
//        printSuccess("User added to group successfully.");
//    }
//    else {
//        printError("Group is full. Cannot add more users.");
//    }
//}
//
//string decompressRLE(const string& input) {
//    string decompressed;
//    size_t i = 0;
//
//    while (i < input.size()) {
//        string numStr;
//        while (i < input.size() && isdigit(static_cast<unsigned char>(input[i]))) {
//            numStr += input[i++];
//        }
//        if (i < input.size()) {
//            char ch = input[i++];
//            if (ch < 0 || ch > 255) {
//                return "";
//            }
//            try {
//                int count = numStr.empty() ? 1 : stoi(numStr);
//                if (count < 0) {
//                    return "";
//                }
//                decompressed.append(count, ch);
//            }
//            catch (const invalid_argument&) {
//                return "";
//            }
//            catch (const out_of_range&) {
//                return "";
//            }
//        }
//        else if (!numStr.empty()) {
//            return "";
//        }
//    }
//
//    return decompressed;
//}
//
//string compressRLE(const string& input) {
//    if (input.empty()) {
//        return input;
//    }
//
//    for (char c : input) {
//        if (c < 0 || c > 255) {
//            printError("Invalid characters in input. Compression aborted.");
//            return input;
//        }
//    }
//
//    string compressed;
//    int count = 1;
//    size_t i = 0;
//
//    while (i < input.size()) {
//        if (i < input.size() - 1 && input[i] == input[i + 1]) {
//            count++;
//        }
//        else {
//            char countStr[20];
//            intToString(count, countStr, 20);
//            compressed += countStr;
//            compressed += input[i];
//            count = 1;
//        }
//        i++;
//    }
//
//    string decompressed = decompressRLE(compressed);
//    if (decompressed != input) {
//        cout << "Debug: Original: " << input << endl;
//        cout << "Debug: Compressed: " << compressed << endl;
//        cout << "Debug: Decompressed: " << decompressed << endl;
//        printError("Compression verification failed. Using original content.");
//        return input;
//    }
//
//    return compressed;
//}
//
//void compressFile() {
//    string fileName;
//    cout << "Enter file name to compress: ";
//    getline(cin, fileName);
//
//    string fullPath = currentPath + fileName;
//    bool found = false;
//
//    for (int i = 0; i < fileCount; i++) {
//        if (files[i].path == fullPath) {
//            found = true;
//            if (files[i].compressed) {
//                printError("File is already compressed.");
//                return;
//            }
//
//            string original = files[i].content;
//            string compressed = compressRLE(original);
//
//            if (compressed == original) {
//                return;
//            }
//
//            files[i].content = compressed;
//            files[i].compressed = true;
//            files[i].compressionType = "RLE";
//
//            char origSizeStr[20], compSizeStr[20];
//            intToString(static_cast<int>(original.size()), origSizeStr, 20);
//            intToString(static_cast<int>(compressed.size()), compSizeStr, 20);
//
//            cout << "\n--- Compression Info ---\n";
//            cout << "Original String:   " << original << endl;
//            cout << "Compressed String: " << compressed << endl;
//
//            printSuccess("File compressed successfully. Size reduced from " +
//                string(origSizeStr) + " to " + string(compSizeStr) + " bytes.");
//            break;
//        }
//    }
//
//    if (!found) {
//        printError("File not found.");
//    }
//}
//
//void decompressFile() {
//    string fileName;
//    cout << "Enter file name to decompress: ";
//    getline(cin, fileName);
//
//    string fullPath = currentPath + fileName;
//    bool found = false;
//
//    for (int i = 0; i < fileCount; i++) {
//        if (files[i].path == fullPath) {
//            found = true;
//            if (!files[i].compressed) {
//                printError("File is not compressed.");
//                return;
//            }
//
//            if (files[i].compressionType == "RLE") {
//                string compressed = files[i].content;
//                string decompressed = decompressRLE(compressed);
//
//                if (decompressed.empty()) {
//                    printError("Decompression failed. File may be corrupted.");
//                    return;
//                }
//
//                files[i].content = decompressed;
//                files[i].compressed = false;
//                files[i].compressionType = "";
//
//                char compSizeStr[20], decompSizeStr[20];
//                intToString(static_cast<int>(compressed.size()), compSizeStr, 20);
//                intToString(static_cast<int>(decompressed.size()), decompSizeStr, 20);
//
//                cout << "\n--- Decompression Info ---\n";
//                cout << "Compressed String:   " << compressed << endl;
//                cout << "Decompressed String: " << decompressed << endl;
//
//                printSuccess("File decompressed successfully. Size increased from " +
//                    string(compSizeStr) + " to " + string(decompSizeStr) + " bytes.");
//            }
//            else {
//                printError("Unknown compression type.");
//            }
//            break;
//        }
//    }
//
//    if (!found) {
//        printError("File not found.");
//    }
//}
//
//void syncWithCloud() {
//    if (currentUser.empty()) {
//        printError("You must be logged in to sync with cloud.");
//        return;
//    }
//
//    setColor(COLOR_CYAN);
//    cout << "\n======== CLOUD SYNC (QUEUE) ========\n";
//    setColor(COLOR_RESET);
//    cout << "Preparing items for cloud sync...\n";
//
//    const int MAX_SYNC_QUEUE = 200;
//    struct SyncTask {
//        char name[100];
//        char type[10];
//        int size;
//    };
//    SyncTask syncQueue[MAX_SYNC_QUEUE];
//    int front = 0, rear = 0;
//
//    auto startsWith = [](const char* str, const char* prefix) {
//        int i = 0;
//        while (prefix[i] != '\0' && str[i] != '\0') {
//            if (str[i] != prefix[i]) return false;
//            i++;
//        }
//        return prefix[i] == '\0';
//        };
//
//    auto compareStrings = [](const char* a, const char* b) {
//        int i = 0;
//        while (a[i] != '\0' && b[i] != '\0') {
//            if (a[i] != b[i]) return a[i] - b[i];
//            i++;
//        }
//        return a[i] - b[i];
//        };
//
//    auto enqueueSyncTask = [&](const char* name, const char* type, int size = 0) {
//        if ((rear + 1) % MAX_SYNC_QUEUE == front) {
//            printError("Sync queue is full.");
//            return;
//        }
//
//        int i = 0;
//        while (name[i] != '\0' && i < 99) {
//            syncQueue[rear].name[i] = name[i];
//            i++;
//        }
//        syncQueue[rear].name[i] = '\0';
//
//        i = 0;
//        while (type[i] != '\0' && i < 9) {
//            syncQueue[rear].type[i] = type[i];
//            i++;
//        }
//        syncQueue[rear].type[i] = '\0';
//
//        syncQueue[rear].size = size;
//        rear = (rear + 1) % MAX_SYNC_QUEUE;
//        };
//
//    int count = 0;
//    for (int i = 0; i < fileCount; i++) {
//        if (startsWith(files[i].path.c_str(), currentPath.c_str())) {
//            int contentLength = static_cast<int>(files[i].content.length());
//            enqueueSyncTask(files[i].name.c_str(), "File", contentLength);
//            count++;
//        }
//    }
//
//    for (int i = 0; i < folderCount; i++) {
//        if (startsWith(folders[i].path.c_str(), currentPath.c_str()) &&
//            compareStrings(folders[i].path.c_str(), currentPath.c_str()) != 0) {
//            enqueueSyncTask(folders[i].name.c_str(), "Folder");
//            count++;
//        }
//    }
//
//    cout << "\nSyncing in background...\n";
//    while (front != rear) {
//        SyncTask task = syncQueue[front];
//        front = (front + 1) % MAX_SYNC_QUEUE;
//
//        cout << "^ ";
//        int k = 0;
//        if (compareStrings(task.type, "File") == 0) {
//            cout << "Uploading File: ";
//            while (task.name[k] != '\0') cout << task.name[k++];
//            cout << " (";
//            char sizeStr[20];
//            intToString(task.size, sizeStr, 20);
//            cout << sizeStr << " bytes)";
//        }
//        else {
//            cout << "Creating Cloud Folder: ";
//            while (task.name[k] != '\0') cout << task.name[k++];
//        }
//        cout << "\n";
//
//        Sleep(200);
//    }
//
//    setColor(COLOR_CYAN);
//    cout << "====================================\n";
//    setColor(COLOR_RESET);
//
//    char countStr[20];
//    intToString(count, countStr, 20);
//    printSuccess("Cloud sync complete. " + string(countStr) + " items synchronized.");
//}
//
//void garbageCollection() {
//    setColor(COLOR_YELLOW);
//    cout << "\n======== GARBAGE COLLECTION ========\n";
//    setColor(COLOR_RESET);
//    cout << "Running garbage collection...\n";
//
//    int foldersRemoved = 0;
//    int versionsRemoved = 0;
//    int filesRemoved = 0;
//
//    for (int i = folderCount - 1; i >= 0; i--) {
//        if (folders[i].path == "/") continue;
//
//        bool hasFiles = false;
//        for (int j = 0; j < fileCount; j++) {
//            bool match = true;
//            int k = 0;
//            while (folders[i].path[k] != '\0' && files[j].path[k] != '\0') {
//                if (folders[i].path[k] != files[j].path[k]) {
//                    match = false;
//                    break;
//                }
//                k++;
//            }
//            if (folders[i].path[k] != '\0') match = false;
//
//            if (match) {
//                hasFiles = true;
//                break;
//            }
//        }
//
//        if (!hasFiles) {
//            for (int j = i; j < folderCount - 1; j++) {
//                folders[j] = folders[j + 1];
//            }
//            folderCount--;
//            foldersRemoved++;
//        }
//    }
//
//    for (int i = 0; i < fileCount; i++) {
//        if (files[i].versionList.count > MAX_VERSIONS) {
//            int toRemove = files[i].versionList.count - MAX_VERSIONS;
//            FileVersionNode* current = files[i].versionList.head;
//            FileVersionNode* prev = nullptr;
//
//            for (int j = 0; j < toRemove; j++) {
//                prev = current;
//                current = current->next;
//                delete prev;
//                versionsRemoved++;
//            }
//
//            files[i].versionList.head = current;
//            files[i].versionList.count = MAX_VERSIONS;
//        }
//    }
//
//    for (int i = 0; i < recycleCount; i++) {
//        freeVersionList(recycleBin[i].file.versionList);
//        if (recycleBin[i].file.graphNode != nullptr) {
//            delete recycleBin[i].file.graphNode;
//            recycleBin[i].file.graphNode = nullptr;
//        }
//        filesRemoved++;
//    }
//    recycleCount = 0;
//
//    string foldersRemovedStr = "";
//    int temp1 = foldersRemoved;
//    if (temp1 == 0) {
//        foldersRemovedStr = "0";
//    }
//    else {
//        while (temp1 > 0) {
//            char digit = '0' + (temp1 % 10);
//            foldersRemovedStr = digit + foldersRemovedStr;
//            temp1 /= 10;
//        }
//    }
//
//    string versionsRemovedStr = "";
//    int temp2 = versionsRemoved;
//    if (temp2 == 0) {
//        versionsRemovedStr = "0";
//    }
//    else {
//        while (temp2 > 0) {
//            char digit = '0' + (temp2 % 10);
//            versionsRemovedStr = digit + versionsRemovedStr;
//            temp2 /= 10;
//        }
//    }
//
//    string filesRemovedStr = "";
//    int temp3 = filesRemoved;
//    if (temp3 == 0) {
//        filesRemovedStr = "0";
//    }
//    else {
//        while (temp3 > 0) {
//            char digit = '0' + (temp3 % 10);
//            filesRemovedStr = digit + filesRemovedStr;
//            temp3 /= 10;
//        }
//    }
//
//    setColor(COLOR_YELLOW);
//    cout << "=================================\n";
//    setColor(COLOR_RESET);
//
//    printSuccess("Garbage collection complete. Removed " +
//        foldersRemovedStr + " empty folders, " +
//        versionsRemovedStr + " old versions, and " +
//        filesRemovedStr + " files from recycle bin.");
//}
//
//void balanceTree() {
//    if (folderCount <= 1) {
//        printWarning("Folder structure is already balanced.");
//        return;
//    }
//
//    setColor(COLOR_CYAN);
//    cout << "\n======== BALANCE FOLDER TREE ========\n";
//    setColor(COLOR_RESET);
//
//    // Display folder structure before balancing
//    cout << "\nFolder Structure Before Balancing:\n";
//    for (int i = 0; i < folderCount; i++) {
//        int depth = getFolderDepth(folders[i].path);
//        for (int j = 0; j < depth * 2; j++) cout << " ";
//        string name = (folders[i].path == "/") ? "/" : folders[i].name;
//        cout << "L-- " << name << " (" << folders[i].path << ")\n";
//    }
//
//    // Perform bubble sort to balance folders alphabetically
//    int swaps = 0;
//    for (int i = 0; i < folderCount - 1; i++) {
//        for (int j = 0; j < folderCount - i - 1; j++) {
//            if (folders[j].name > folders[j + 1].name) {
//                Folder temp = folders[j];
//                folders[j] = folders[j + 1];
//                folders[j + 1] = temp;
//                swaps++;
//            }
//        }
//    }
//
//    // Display folder structure after balancing
//    cout << "\nFolder Structure After Balancing:\n";
//    for (int i = 0; i < folderCount; i++) {
//        int depth = getFolderDepth(folders[i].path);
//        for (int j = 0; j < depth * 2; j++) cout << " ";
//        string name = (folders[i].path == "/") ? "/" : folders[i].name;
//        cout << "L-- " << name << " (" << folders[i].path << ")\n";
//    }
//
//    // Convert swaps to string for output
//    char swapsStr[20];
//    intToString(swaps, swapsStr, 20);
//
//    setColor(COLOR_CYAN);
//    cout << "====================================\n";
//    setColor(COLOR_RESET);
//
//    if (swaps == 0) {
//        printWarning("No changes made. Folder structure was already balanced.");
//    }
//    else {
//        printSuccess("Folder structure balanced alphabetically. Made " + string(swapsStr) + " swap(s).");
//    }
//}
//void exitSystem() {
//    setColor(COLOR_CYAN);
//    cout << "\n======== SYSTEM SHUTDOWN ========\n";
//    setColor(COLOR_RESET);
//    cout << "Saving system state...\n";
//
//    GraphNode* node = graphNodes;
//    while (node != nullptr) {
//        GraphNode* temp = node;
//        node = node->next;
//        delete temp;
//    }
//    graphNodes = nullptr;
//
//    GraphEdge* edge = graphEdges;
//    while (edge != nullptr) {
//        GraphEdge* temp = edge;
//        edge = edge->next;
//        delete temp;
//    }
//    graphEdges = nullptr;
//
//    for (int i = 0; i < fileCount; i++) {
//        freeVersionList(files[i].versionList);
//    }
//
//    cout << "System state saved. Exiting...\n";
//    setColor(COLOR_CYAN);
//    cout << "=================================\n";
//    setColor(COLOR_RESET);
//    exit(0);
//}
//
//// Menu Functions
//void displayMainMenu() {
//    printHeader("GOOGLE DRIVE FILE SYSTEM");
//    printCurrentUserInfo();
//
//    setColor(COLOR_BLUE);
//    cout << endl << " MAIN MENU :" << endl;
//    setColor(COLOR_RESET);
//
//    printMenuOption(1, "User Account Operations");
//    printMenuOption(2, "File Operations");
//    printMenuOption(3, "Folder Operations");
//    printMenuOption(4, "File Sharing");
//    printMenuOption(5, "Administrative Functions");
//    printMenuOption(6, "Help & Documentation");
//    printMenuOption(0, "Exit System");
//
//    cout << endl;
//}
//
//void displayUserMenu() {
//    printHeader("USER ACCOUNT");
//    printCurrentUserInfo();
//
//    setColor(COLOR_BLUE);
//    cout << endl << " USER MENU:" << endl;
//    setColor(COLOR_RESET);
//    printMenuOption(1, "Register User");
//    printMenuOption(2, "Login User");
//    printMenuOption(3, "Logout User");
//    printMenuOption(4, "Reset Password");
//    printMenuOption(0, "Back to Main Menu");
//    cout << endl;
//}
//
//void displayFileMenu() {
//    printHeader("FILE OPERATIONS");
//    printCurrentUserInfo();
//
//    setColor(COLOR_BLUE);
//    cout << endl << " FILE MENU:" << endl;
//    setColor(COLOR_RESET);
//
//    printMenuOption(1, "Create File");
//    printMenuOption(2, "Read File");
//    printMenuOption(3, "Update File");
//    printMenuOption(4, "Delete File");
//    printMenuOption(5, "View Recycle Bin");
//    printMenuOption(6, "Restore File");
//    printMenuOption(7, "Empty Recycle Bin");
//    printMenuOption(8, "View Recent Files");
//    printMenuOption(9, "Search Files");
//    printMenuOption(10, "View Version History");
//    printMenuOption(11, "Rollback Version");
//    printMenuOption(12, "Compress File");
//    printMenuOption(13, "Decompress File");
//    printMenuOption(0, "Back to Main Menu");
//
//    cout << endl;
//}
//
//void displayFolderMenu() {
//    printHeader("FOLDER OPERATIONS");
//    printCurrentUserInfo();
//
//    setColor(COLOR_BLUE);
//    cout << "\n FOLDER MENU:\n";
//    setColor(COLOR_RESET);
//
//    printMenuOption(1, "Create Folder");
//    printMenuOption(2, "Delete Folder");
//    printMenuOption(3, "Navigate Folder");
//    printMenuOption(4, "View File System Tree (DFS)");
//    printMenuOption(5, "View File System Tree (BFS)");
//    printMenuOption(0, "Back to Main Menu");
//
//    cout << endl;
//}
//
//void displaySharingMenu() {
//    printHeader("FILE SHARING");
//    printCurrentUserInfo();
//
//    setColor(COLOR_BLUE);
//    cout << endl << " SHARING MENU:" << endl;
//    setColor(COLOR_RESET);
//
//    printMenuOption(1, "Share File");
//    printMenuOption(2, "Revoke Access");
//    printMenuOption(0, "Back to Main Menu");
//
//    cout << endl;
//}
//
//void displayAdminMenu() {
//    printHeader("ADMINISTRATION");
//    printCurrentUserInfo();
//
//    setColor(COLOR_BLUE);
//    cout << "\n ADMIN MENU:\n";
//    setColor(COLOR_RESET);
//
//    printMenuOption(1, "Set File Permissions");
//    printMenuOption(2, "Create User Group");
//    printMenuOption(3, "Add User to Group");
//    printMenuOption(4, "Sync with Cloud");
//    printMenuOption(5, "Balance Folder Tree");
//    printMenuOption(6, "Garbage Collection");
//    printMenuOption(0, "Back to Main Menu");
//
//    cout << endl;
//}
//
//void displayHelpMenu() {
//    printHeader("HELP & DOCUMENTATION");
//
//    setColor(COLOR_BLUE);
//    cout << "\n FILE SYSTEM COMMANDS:\n";
//    setColor(COLOR_RESET);
//
//    cout << " - Use the menu numbers to select operations\n";
//    cout << " - Only admins can access administrative functions\n";
//    cout << " - Files are automatically versioned when updated\n";
//    cout << " - Deleted files go to the Recycle Bin\n";
//    cout << " - Compressed files cannot be edited until decompressed\n";
//
//    setColor(COLOR_BLUE);
//    cout << "\n CURRENT LIMITS:\n";
//    setColor(COLOR_RESET);
//
//    cout << " - Max users: " << MAX_USERS << "\n";
//    cout << " - Max files: " << MAX_FILES << "\n";
//    cout << " - Max versions per file: " << MAX_VERSIONS << "\n";
//    cout << " - Max recent files: " << MAX_RECENT << "\n";
//
//    setColor(COLOR_YELLOW);
//    cout << "\nPress 0 to return to main menu\n";
//    setColor(COLOR_RESET);
//}
//
//int main() {
//    initializeFileSystem();
//
//    while (true) {
//        bool isAdmin = false;
//        displayMainMenu();
//        int mainChoice = getValidChoice(6);
//
//        switch (mainChoice) {
//        case 1: // User Account Operations
//            while (true) {
//                displayUserMenu();
//                int subChoice = getValidChoice(4);
//
//                switch (subChoice) {
//                case 1: registerUser(); break;
//                case 2: loginUser(); break;
//                case 3: logoutUser(); break;
//                case 4: resetPassword(); break;
//                case 0: break;
//                default: printError("Invalid option. Please try again.");
//                }
//                if (subChoice == 0) break;
//            }
//            break;
//
//        case 2: // File Operations
//            while (true) {
//                displayFileMenu();
//                int subChoice = getValidChoice(13);
//
//                switch (subChoice) {
//                case 1: createFile(); break;
//                case 2: readFile(); break;
//                case 3: updateFile(); break;
//                case 4: deleteFile(); break;
//                case 5: viewRecycleBin(); break;
//                case 6: restoreFile(); break;
//                case 7: emptyRecycleBin(); break;
//                case 8: viewRecentFiles(); break;
//                case 9: searchFiles(); break;
//                case 10: viewVersionHistory(); break;
//                case 11: rollbackVersion(); break;
//                case 12: compressFile(); break;
//                case 13: decompressFile(); break;
//                case 0: break;
//                default: printError("Invalid option. Please try again.");
//                }
//                if (subChoice == 0) break;
//            }
//            break;
//
//        case 3: // Folder Operations
//            while (true) {
//                displayFolderMenu();
//                int subChoice = getValidChoice(5);
//
//                switch (subChoice) {
//                case 1: createFolder(); break;
//                case 2: deleteFolder(); break;
//                case 3: navigateFolder(); break;
//                case 4: treeTraversal(true); break;
//                case 5: treeTraversal(false); break;
//                case 0: break;
//                default: printError("Invalid option. Please try again.");
//                }
//                if (subChoice == 0) break;
//            }
//            break;
//
//        case 4: // File Sharing
//            while (true) {
//                displaySharingMenu();
//                int subChoice = getValidChoice(2);
//
//                switch (subChoice) {
//                case 1: shareFile(); break;
//                case 2: revokeAccess(); break;
//                case 0: break;
//                default: printError("Invalid option. Please try again.");
//                }
//                if (subChoice == 0) break;
//            }
//            break;
//
//        case 5: // Administrative Functions
//            if (currentUser.empty()) {
//                printError("You must be logged in to access administrative functions.");
//                break;
//            }
//            for (int i = 0; i < userCount; i++) {
//                if (users[i].username == currentUser && users[i].accessLevel == "admin") {
//                    isAdmin = true;
//                    break;
//                }
//            }
//            if (!isAdmin) {
//                printError("Administrative functions require admin privileges.");
//                break;
//            }
//
//            while (true) {
//                displayAdminMenu();
//                int subChoice = getValidChoice(6);
//
//                switch (subChoice) {
//                case 1: setPermissions(); break;
//                case 2: createUserGroup(); break;
//                case 3: addUserToGroup(); break;
//                case 4: syncWithCloud(); break;
//                case 5: balanceTree(); break;
//                case 6: garbageCollection(); break;
//                case 0: break;
//                default: printError("Invalid option. Please try again.");
//                }
//                if (subChoice == 0) break;
//            }
//            break;
//
//        case 6: // Help
//            displayHelpMenu();
//            getValidChoice(0);
//            break;
//
//        case 0: // Exit
//            exitSystem();
//            break;
//
//        default:
//            printError("Invalid option. Please try again.");
//        }
//    }
//    system("pause");
//    return 0;
//}