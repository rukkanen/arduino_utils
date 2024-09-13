# **Universal High-Level Storage System**

This storage system is designed to manage memory on an **Arduino Micro** (or similar platform) using **SRAM**. The goal is to provide a **high-level interface** for managing data without needing to manually track memory addresses. The system manages different types of data, like **key-value pairs**, **lists**, and **other structured data**, and provides functionality such as **defragmentation**, **transient data handling**, and **reinitialization**.

If this seems familiar to you, it might be that I guess I just specified a harddrive in a computer. But this is a **SRAM** storage system, which is a bit different.

## **Key Components**

### **1. Memory Layout**
- **Meta Block**: Defines the system's overall structure.
  - Holds information about the format version, block sizes, TOC location, transient settings, and whether SRAM is formatted.
- **TOC Block (Table of Contents)**: Manages metadata for each block of dynamic memory.
  - Contains details like the **free/used status**, **block size**, **key**, and **max_age** for transient blocks.
- **Dynamic Memory Block**: The main storage area for actual data.
  - Contains key-value pairs, lists, sensor data, SLAM maps, or other application-specific data.

### **2. Key Aspects of the System**

#### **2.1 Key-Based and Indexed Access**
- **Keys** can represent single values (e.g., `"config_item"`) or lists (e.g., `"config_item[3]"`).
- Data can be accessed by **key** or **index**, allowing for flexible interaction with stored data.
- The TOC maps keys to blocks and tracks where they are stored in the dynamic memory.

#### **2.2 Automatic Memory Allocation**
- When data is written, the system automatically finds a free block in memory.
- A check ensures that the data will fit in the available space. If not, the write operation fails gracefully.
- **Block size** is managed dynamically, with a **minimum block size** defined (e.g., 20 bytes for a key-value pair).

#### **2.3 Overwriting Data**
- Before writing, the system checks if the key already exists.
- If the key is found, the system returns a status like `key_exists`, prompting the user to call a **separate overwrite function**.
- This ensures intentional overwriting of data and prevents accidental loss.

#### **2.4 Handling Transient Blocks**
- Transient blocks can be marked to be **automatically freed** when their **max_age** is reached.
- **max_age** can be flexible, though for now it’s defined as a placeholder, allowing for future cyclic garbage collection.
- **Transient blocks** are especially useful for temporary data, like sensor logs or intermediate messages.

#### **2.5 Defragmentation**
- Memory can become fragmented over time as blocks are written, erased, and reallocated.
- The system will provide a **defragmentation method** that moves fragmented data into contiguous blocks.
- **Fragmentation status** can be reported with a **0.0 to 1.0 value**, representing how fragmented the memory is, where `0.0` means no fragmentation and `1.0` means fully fragmented.
- Defragmentation could be triggered **automatically** when the fragmentation value crosses a certain threshold, or manually by calling a defrag function.

#### **2.6 Reinitialization Options**
- **Wiping the Meta Block**: Resets the entire system, marking everything as free to use.
  - This clears the TOC and data, essentially "reformatting" the SRAM.
- **Wiping the TOC Block**: Keeps the data in SRAM but marks all slots as free. Useful for a soft reset of the system without losing the underlying memory structure.
- **Zeroing Dynamic Blocks**: A method will be available to write zeroes to specific blocks of dynamic memory, ensuring they are cleared without needing to wipe the entire dynamic memory.

### **3. Block Management**
- **Block Size**: Each block has a minimum size (e.g., 20 bytes for key-value pairs). Larger blocks can be dynamically allocated based on the data.
- **TOC Entries**: Each entry in the TOC tracks the following:
  - **Free/Used Status**: Marks whether a block is available or in use.
  - **Block Size**: How many bytes the block occupies.
  - **Key**: The string identifier associated with the block (like `"config_item"`).
  - **Data Type**: Information on whether the block contains a single value, a list, SLAM data, etc.
  - **Transient Flag**: If the block is marked as transient, it will be freed when `max_age` is reached.

### **4. Error Handling**
- Since Arduino doesn’t support exceptions, the system will rely on **error codes** and **boolean flags** to indicate success or failure.
- Possible error conditions include:
  - **key_exists**: Returned if an attempt is made to write to an already existing key.
  - **write_failed**: Returned if a write operation cannot complete due to insufficient space or fragmentation.

---

## **Planned Functions**

### **1. Memory Management Functions**
- **Store Key-Value Pairs**: Handles writing key-value pairs or lists into memory.
- **Overwrite Key**: Provides explicit functionality to overwrite existing data when necessary.
- **Query for Key**: Checks whether a key already exists in the TOC.
- **Free Transient Data**: Cycles through the TOC to remove transient data when the `max_age` is reached.
- **Defragment Memory**: Moves data into contiguous blocks and provides a fragmentation status.

### **2. System Maintenance Functions**
- **Wipe Meta Block**: Fully resets the system by clearing all memory structures and data.
- **Wipe TOC Block**: Clears the TOC, marking all blocks as free while retaining the data in SRAM.
- **Zero Dynamic Block**: Allows a specific block to be zeroed out without wiping the entire dynamic memory area.

### **3. Memory Checking**
- **Fragmentation Status**: Provides a 0.0 to 1.0 value indicating the current level of memory fragmentation.
- **Max Age Handling**: Future-proof method that could use a cyclic approach to free data based on `max_age`.

---

## **Outstanding Questions**
1. **Defragmentation Triggers**: Should defragmentation be **manual** or **automatic** based on memory fragmentation levels?
2. **Max Age**: How exactly should `max_age` be handled? Should it be a relative timeout, or do you prefer some other method for transient data cleanup?
3. **Block Sizing**: Should we use **fixed** or **variable block sizes** for certain types of data, or let the user decide during the writing process?

---

This markdown summarizes our entire discussion and design planning for the **universal high-level storage system**. Let’s revisit and refine anything you want before diving into the actual code implementation.
