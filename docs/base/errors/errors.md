# errors.h
## macros

- GenericVal_ErrorCode(name)
    defines an error code
- GenericValImpl_ErrorCode(name)
    generates an error code. The error code is a pointer to **StringView** containing the name as text
## types

### ErrorCode
``` C
typedef const StringView* ErrorCode;
```
unique code for each error or error group. is a pointer to **StringView**, meaning the code can also point to a string with more information.

can be generated with macros:
 - **GenericVal_ErrorCode()** for definition
 - **GenericValImpl_ErrorCode()** for implementation

---
 ### Error
``` C
typedef struct {
    const ErrorCode grp_code;
    const ErrorCode code;
    const StringView msg;
} Error;
```
 
represents an error.
is used trough **C_Result** for passing information about any errors that ocured.
reason for the error can be found by checking its error group code or code.
#### related functions
- Error Error_construct(ErrorCode grp_code, ErrorCode code, StringView msg)  

    ``` C
    self.grp_code = grp_code;
    self.code = code;
    self.msg = msg;
    ``` 

    **see:** C_Result
