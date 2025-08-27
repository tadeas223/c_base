# **C_HashTable** : **ClassObject**
**package:** [ds](ds.md)

**implements:**  
- **IHashable**: `C_HashTable_equals`, `C_HashTable_hash`
- **IFormattable**: `C_HashTable_to_str_R`, `C_HashTable_to_str_format_R`

---

## **overview**
`C_HashTable` stores objects that extend `ClassObject`.
This implementation uses `IHashable` interface of the objects to hash them.
If the stored object does not implement the interface its pointer is used instead of the hash.

- Stores references and manages ownership (with ref/unref)
- Not thread-safe
- Append is **O(N)**, prepend is **O(1)**

---
## **functions**

### **C_HashTable\* C_HashTable_new(void)**
> *tested*

Creates and initializes a new `C_HashTable` instance.

**returns:**
- `C_HashTable*`: New instance of `C_HashTable`

---

### **void C_HashTable_destroy(void\* self)**
> *tested*

Destroys the hash table and unreferences all values.

**params:**
- `self`: Pointer to the **C_HashTable** instance

**notes:**
- The hash table must not be used after this call.

---
### **void C_HashTable_put_P(C_HashTable\* self, void\* key, void\* value)**
> *tested*

Adds a value and connects it to the key.
*crashes:*
- E(EG_Datastructures, E_InvalidPointer, ...)
    if `key` is already stored in the hash table

**params:**
- `key`: Key that will be used for indexing
- `value`: Value to store (reference is added)

---
### **void\* C_HashTable_at_PB(C_HashTable\* self, void\* key)**
> *tested*

Borrows the value at the specified key.

**crashes:**
- `E(EG_Datastructures, E_InvalidPointer, ...)`:
    if the hash table does not contain the key

**params:**
- `key`: key of the value

**returns:**
- `void*`: Borrowed value 

---
### **void\* C_HashTable_at_PB(C_HashTable\* self, void\* key)**
> *tested*

Returns a reference to value at the specified key.

**crashes:**
- `E(EG_Datastructures, E_InvalidPointer, ...)`:
    if the hash table does not contain the key

**params:**
- `key`: key of the value

**returns:**
- `void*`: Owned value 

---

### **void\* C_HashTable_remove_R(C_HashTable\* self, u32 index)**
> *tested*

Removes a value at the given key.

**crashes:**
- E(EG_Datastructures, E_InvalidPointer, ...)
    if the `key` is not stored in the hash table   

**params:**
- `key`: key of the value to remove

**returns:**
- `void*`: Referenced removed value
---

### **void C_HashTable_clear(C_HashTable\* self)**
> *tested*

Removes all elements and unreferences them.

- Hash table remains valid after clearing.

---
### **u32 C_HashTable_hash(C_HashTable\* self)**
> *not tested*: cannot test

Hashes the hash table by hashing all its elements and adding it together.

**notes:**
- the hash changes each time new elements are added or removed from the hash table.

**returns:**
- `u32`: hash of the hash table

---
### **bool C_HashTable_equals(void\* a, void\* b)**
> *tested*

Checks all elements in both hash tables. If the hash tables contain the same elements this function returns true.

**returns:**
- `bool`: true if hash tables are equal, false otherwise

---
### **C_String\* C_HashTable_to_str_format_R(void\* self, C_String\* format)**
> *not tested*: cannot test

Converts the hash table into a string.

**format:**
``` format
{start}key{el_sep}value{sep}key{el_sep}value{sep}key{el_sep}value{end}
```

**returns:** 
- `C_String*`: hash table converted into a string

---
### **C_String\* C_HashTable_to_str_R(void\* self, C_String\* format)**
> *not tested*: format may change in the future

Converts the hash table into a string.
``` C
C_HashTable_to_str_format_R(self, S("start=[;end=];el_sep= : ;sep=, ")
```

**format:**
``` format
[key : value, key : value, key : value]
```

**returns:** 
- `C_String*`: hash table converted into a string

---
### **u32 C_HashTable_get_cap(C_HashTable\* self)**
> *not tested*: too simple

Returns the bucket capacity of the hash table.
Hash table can hold more than this capacity.

**returns:**
- `u32`: Bucket capacity of the hash table

