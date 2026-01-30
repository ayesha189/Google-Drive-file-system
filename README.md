# Google Drive File System 📁

A C++ implementation of a Google Drive-like file system built as a Data Structures and Algorithms project. This system demonstrates the practical application of fundamental data structures including Trees, Graphs, Stacks, and Queues to create a hierarchical file management system with file sharing capabilities.


## 🎯 Overview

This project implements a Google Drive-like file system in C++ using various data structures to efficiently manage files and folders. The system supports hierarchical organization, file operations, file sharing, and traversal algorithms.

### Key Objectives

- Implement a hierarchical file system structure using Trees
- Demonstrate practical applications of core data structures (Tree, Graph, Stack, Queue)
- Apply BFS and DFS algorithms for directory traversal
- Simulate file sharing using Graph data structure
- Create and manage files and folders efficiently

## ✨ Features

### Core Functionality

- **Hierarchical Structure**: Tree-based folder organization with parent-child relationships
- **File Creation**: Create and store files with content
- **Folder Creation**: Create nested folders and subdirectories
- **Directory Traversal**: Navigate through the folder structure
- **File Sharing**: Share files between users using Graph data structure
- **BFS Traversal**: Level-order traversal of directory structure
- **DFS Traversal**: Depth-first traversal for nested folder exploration
- **Path Navigation**: Support for directory path operations

## 🔧 Data Structures Used

### 1. **Tree (N-ary Tree)**
- **Purpose**: Represents the hierarchical folder structure
- **Implementation**: Each folder node can have multiple children (files and subfolders)
- **Operations**: Insertion, traversal (BFS/DFS), search
- **Application**: Root directory with nested folders and files

### 2. **Graph**
- **Purpose**: Manages file sharing relationships between users
- **Type**: Directed graph representing sharing connections
- **Implementation**: Adjacency list or adjacency matrix
- **Operations**: Add edge (share file), traverse (find shared files)
- **Application**: User-to-user file sharing network

### 3. **Stack**
- **Purpose**: Used for Depth-First Search (DFS) traversal
- **Operations**: Push, pop for exploring directory depth
- **Application**: Recursive directory exploration and path tracking

### 4. **Queue**
- **Purpose**: Used for Breadth-First Search (BFS) traversal
- **Operations**: Enqueue, dequeue for level-order traversal
- **Application**: Level-wise directory listing and searching

## 🏗️ System Architecture

```
google-drive-fs/
├── main.cpp           # Main program with all implementations
├── input.txt          # Sample input file for testing
└── README.md          # Project documentation
```

### Project Structure

- **main.cpp**: Contains all class definitions, data structure implementations, and main function
  - Tree node structure for files/folders
  - Graph implementation for file sharing
  - Stack and Queue for traversal algorithms
  - BFS and DFS implementations
  - File and folder creation functions
  
- **input.txt**: Contains test data or sample directory structure for demonstration

- **README.md**: Comprehensive project documentation

## 💻 Installation

### Prerequisites

- C++ Compiler (GCC, g++, MinGW, or Visual Studio)
- C++11 or higher

### Compilation

```bash
# Clone the repository
git clone https://github.com/yourusername/google-drive-fs.git

# Navigate to project directory
cd google-drive-fs

# Compile the program
g++ main.cpp -o filesystem

# Run the program
./filesystem

# On Windows
filesystem.exe
```

## 🚀 Usage

### Program Flow

The program provides a menu-driven interface or executes predefined operations to demonstrate the file system functionality.

### Sample Operations

```cpp
// Create root folder
createFolder("Root");

// Create subfolders
createFolder("Documents");
createFolder("Pictures");
createFolder("Music");

// Create files
createFile("report.txt", "Root/Documents");
createFile("photo.jpg", "Root/Pictures");

// Share a file between users
shareFile("report.txt", "User1", "User2");

// Traverse directory using BFS
BFS(rootNode);

// Traverse directory using DFS
DFS(rootNode);

// Display file sharing network
displaySharingGraph();
```

## 📚 Core Functions

### File and Folder Operations

#### `createFolder(string folderName)`
Creates a new folder in the file system tree structure.

#### `createFile(string fileName, string path)`
Creates a new file at the specified path in the directory tree.

#### `displayDirectory()`
Displays the complete directory structure.

### Sharing Operations

#### `shareFile(string fileName, string fromUser, string toUser)`
Creates a sharing relationship between users using graph data structure.

#### `displaySharedFiles(string userName)`
Shows all files shared with or by a specific user.

### Traversal Operations

#### `BFS(Node* root)`
Performs Breadth-First Search traversal of the directory tree.
- Uses Queue data structure
- Level-order traversal
- Displays directories level by level

#### `DFS(Node* root)`
Performs Depth-First Search traversal of the directory tree.
- Uses Stack data structure (or recursion)
- Pre-order traversal
- Explores depth before breadth

## 🔍 Algorithms Implemented

### 1. Breadth-First Search (BFS)
- **Purpose**: Level-order traversal of directory structure
- **Data Structure**: Queue
- **Application**: Display all files and folders level by level
- **Time Complexity**: O(n) where n is the number of nodes

```cpp
void BFS(Node* root) {
    if (!root) return;
    
    queue<Node*> q;
    q.push(root);
    
    while (!q.empty()) {
        Node* current = q.front();
        q.pop();
        
        cout << current->name << endl;
        
        for (Node* child : current->children) {
            q.push(child);
        }
    }
}
```

### 2. Depth-First Search (DFS)
- **Purpose**: Deep traversal of nested folder structure
- **Data Structure**: Stack (implicit via recursion or explicit)
- **Application**: Explore nested directories completely before moving to siblings
- **Time Complexity**: O(n) where n is the number of nodes

```cpp
void DFS(Node* root) {
    if (!root) return;
    
    cout << root->name << endl;
    
    for (Node* child : root->children) {
        DFS(child);
    }
}
```

### 3. Graph Traversal for File Sharing
- **Purpose**: Find all users connected through file sharing
- **Implementation**: Adjacency list representation
- **Application**: Determine file sharing network and relationships

## 💡 Example Output

```
========================================
   GOOGLE DRIVE FILE SYSTEM
========================================

Creating Root Directory...
Root created successfully!

Creating Folders...
✓ Documents folder created
✓ Pictures folder created
✓ Music folder created
✓ Videos folder created

Creating Files...
✓ report.txt created in Documents
✓ notes.txt created in Documents
✓ photo1.jpg created in Pictures
✓ song.mp3 created in Music

========================================
BFS TRAVERSAL (Level-Order):
========================================
Level 0: Root
Level 1: Documents Pictures Music Videos
Level 2: report.txt notes.txt photo1.jpg song.mp3

========================================
DFS TRAVERSAL (Depth-First):
========================================
Root
  Documents
    report.txt
    notes.txt
  Pictures
    photo1.jpg
  Music
    song.mp3
  Videos

========================================
FILE SHARING OPERATIONS:
========================================
User1 shares 'report.txt' with User2
User2 shares 'photo1.jpg' with User3
User1 shares 'song.mp3' with User3

Sharing Graph:
User1 --> User2, User3
User2 --> User3

Files shared by User1: report.txt, song.mp3
Files shared with User2: report.txt
Files shared with User3: photo1.jpg, song.mp3
```

## 🔮 Future Enhancements

- [ ] Add file deletion functionality
- [ ] Implement file move and copy operations
- [ ] Add file search by name or type
- [ ] Implement permission levels for file sharing (read, write, admin)
- [ ] Add timestamp for file creation and modification
- [ ] Implement file size tracking
- [ ] Add undo/redo functionality using stack
- [ ] Save and load file system state to/from files
- [ ] Add user authentication system
- [ ] Implement file versioning
- [ ] Create a GUI interface using Qt or similar framework
- [ ] Add support for file encryption

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

### Steps to Contribute

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👨‍💻 Author

**Your Name**
- GitHub: [@yourusername](https://github.com/yourusername)
- LinkedIn: [Your LinkedIn](https://linkedin.com/in/yourprofile)
- Email: your.email@example.com

## 🙏 Acknowledgments

- Data Structures and Algorithms course materials
- Google Drive for inspiration
- Open source community for various resources and tools

---

⭐ If you found this project helpful, please consider giving it a star!

**Note**: This is an educational project created to demonstrate the practical application of data structures. It is not intended for production use.
