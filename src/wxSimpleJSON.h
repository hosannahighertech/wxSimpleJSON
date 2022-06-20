#ifndef WX_SIMPLE_JSON_H
#define WX_SIMPLE_JSON_H

#include <wx/sharedptr.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/filename.h>
#include <wx/dlimpexp.h>
#include <wx/ffile.h>
#include <vector>

#ifdef API_CREATING_DLL
#    define API_EXPORT WXEXPORT
#elif defined(WXUSINGDLL)
#    define API_EXPORT WXIMPORT
#else /* not making nor using DLL */
#    define API_EXPORT
#endif

typedef struct cJSON cJSON;

/// @brief Class for reading, parsing, and writing JSON data.
class API_EXPORT wxSimpleJSON
{
  protected:
    cJSON *m_d{ nullptr};
    bool m_canDelete{ false };

  public:

    /// @brief The data values that JSON supports.
    enum JSONType
    {
        /// @brief Value is invalid.
        IS_INVALID = 0,
        /// @brief Boolean @c false.
        IS_FALSE = 1,
        /// @brief Boolean @c true.
        IS_TRUE = 2,
        /// @brief Null value.
        IS_NULL = 4,
        /// @brief A numeric value.
        IS_NUMBER = 8,
        /// @brief A string value.
        IS_STRING = 16,
        /// @brief An array of values.
        IS_ARRAY = 32,
        /// @brief An object.
        IS_OBJECT = 64,
        /// @brief Raw data.
        IS_RAW = 128
    };
    virtual ~wxSimpleJSON();

    /// @brief A shared wxSimpleJSON pointer.
    /// @details This is what is usually returned while traversing JSON data.
    using Ptr_t = wxSharedPtr<wxSimpleJSON>;

  private:
    // Constructor is private. The way to create an object is by using the
    // Create() or LoadFile() methods.
	wxSimpleJSON();
    /**
     * @brief The deleter pointer
     */
    static void Destroy(wxSimpleJSON *obj);
    static wxSimpleJSON::Ptr_t Create(cJSON *p, bool canDelete = false);

  public:
    // Custom object generators
    /**
     * @brief create a new JSON node.
     * @param type The node type.
     * @param isRoot @c true when creating the top-level item (the root item of the JSON).
     * @note Check returned the object by calling IsNull() or IsOk()
     */
    static wxSimpleJSON::Ptr_t Create(wxSimpleJSON::JSONType, bool isRoot = false);

    /**
     * @brief Parse and return a wxSimpleJSON object.
     * @param buffer A string buffer of JSON data.
     * @param isRoot @c true when creating the top-level item (the root item of the JSON).
     * @param conv The (optional) encoding to read the text with.
     * @note Check returned the object by calling IsNull() or IsOk().
     * @return A wxSimpleJSON object.
     */
    static wxSimpleJSON::Ptr_t Create(const wxString &buffer, bool isRoot = false,
                                      const wxMBConv &conv = wxConvUTF8);

    /**
     * @brief Reads JSON content from a file, parses it, and returns a
            wxSimpleJSON object that can be traversed.
     * @param filename The file path of the JSON file to load.
     * @param conv The (optional) encoding to read the file with.
     * @note Check returned the object by calling IsNull() or IsOk().
     * @return A wxSimpleJSON object.
     */
    static wxSimpleJSON::Ptr_t LoadFile(const wxFileName &filename, const wxMBConv &conv = wxConvUTF8);

    /**
     * @brief Saves the content of this object to a file.
     * @param filename The file path of the JSON file to load.
     * @param conv The (optional) encoding to save the file with.
     * @return @c true if the file save succeeded; @c false otherwise.
     */
    bool Save(const wxFileName &filename, const wxMBConv &conv = wxConvUTF8);

    /**
     * @brief Is this object/node null?
     * @return @c true if the object is null.
     */
    inline bool IsNull() const { return (m_d == nullptr); }

	/**
     * @brief Is this object/node valid (non-null)?
     * @return @c true if the object is valid.
     */
    inline bool IsOk() const { return (m_d != nullptr); }

    // Array manipulation
    wxSimpleJSON &ArrayAdd(wxSimpleJSON::Ptr_t obj);
    wxSimpleJSON &ArrayAdd(const wxString &value, const wxMBConv &conv = wxConvUTF8);
    wxSimpleJSON &ArrayAdd(double value);
    wxSimpleJSON &ArrayAdd(bool value);
    wxSimpleJSON &ArrayAdd(const wxArrayString &arr, const wxMBConv &conv = wxConvUTF8);

    /**
     * @brief Returns the array size.
     * @return The number of items in the array, or 0 if this object
            is not an array (or empty).
     */
    size_t ArraySize() const;
    /**
     * @brief Returns an array item at a given position.
     * @param index Item position in the array.
     * @return A wxSimpleJSON::Ptr_t node (call IsNull() or IsOk() to test for null).
     */
    wxSimpleJSON::Ptr_t Item(size_t index) const;

    // Object manipulation
    /**
     * @brief Adds a property to the node with another node's content.
     * @param name The name of the node to add.
     * @param obj The other node (along with its properties and values) to add.
     * @note If a property with the same name already exists, it will be
     *      replaced with this one.
     * @return A self reference to the node.
     */
    wxSimpleJSON &Add(const wxString &name, wxSimpleJSON::Ptr_t obj);
    /**
     * @brief Adds a property to the node with a string value.
     * @param name The name of the node to add.
     * @param value The string value to assign to the property.
     * @param conv The (optional) encoding to use for the string.
     * @note If a property with the same name already exists, it will be
     *      replaced with this one.
     * @return A self reference to the node.
     */
    wxSimpleJSON &Add(const wxString &name, const wxString &value,
                      const wxMBConv &conv = wxConvUTF8);
    /**
     * @brief Adds a property to the node with a double (i.e., numeric) value.
     * @param name The name of the node to add.
     * @param value The numeric value to assign to the property.
     * @note If a property with the same name already exists, it will be
     *      replaced with this one.
     * @return A self reference to the node.
     */
    wxSimpleJSON &Add(const wxString &name, double value);
    /**
     * @brief Adds a property to the node with a boolean value.
     * @param name The name of the node to add.
     * @param value The value to assign to the property.
     * @note If a property with the same name already exists, it will be
     *      replaced with this one.
     * @return A self reference to the node.
     */
    wxSimpleJSON &Add(const wxString &name, bool value);
    /**
     * @brief Adds a property to the node with a null value.
     * @param name The name of the node to add.
     * @note If a property with the same name already exists, it will be
     *      replaced with this one.
     * @return A self reference to the node.
     */
    wxSimpleJSON &AddNull(const wxString &name);
    /**
     * @brief Adds a property to the node with an array of strings as its value.
     * @param name The name of the node to add.
     * @param arr The array of strings being uses as the property's values.
     * @param conv The (optional) encoding to write the strings with.
     * @note If a property with the same name already exists, it will be
     *      replaced with this one.
     * @return A self reference to the node.
     */
    wxSimpleJSON &Add(const wxString &name, const wxArrayString &arr,
                      const wxMBConv &conv = wxConvUTF8);

    /**
     * @brief Return the node's value as a string (if it is a string).
     * @param conv How to encode the value while reading it.
     * @return The string value that was read, or empty string upon failure.
     * @note Call GetType() to verify the node's data type to ensure that
     *      you are calling the correct @c GetValue___() function.
     */
    wxString GetValueString(const wxMBConv &conv = wxConvUTF8) const;

    /**
     * @brief Return the node's value as a boolean (if it is boolean).
     * @param defaultValue The value to return if reading it as a
     *      boolean failed.
     * @return The value as a boolean.
     * @note Call GetType() to verify the node's data type to ensure that
     *      you are calling the correct @c GetValue___() function.
     */
    bool GetValueBool(bool defaultValue = false) const;

    /**
     * @brief Returns the node's values as an array of strings
            (if that is its type).
     * @param conv How to encode the values while reading them.
     * @return The node's values as a string array.
     * @note Call GetType() to verify the node's data type to ensure that
     *      you are calling the correct @c GetValue___() function.
     */
    wxArrayString GetValueArrayString(const wxMBConv &conv = wxConvUTF8) const;
    /**
     * @brief Returns the node's values as an array of strings
            (if that is its type).
     * @param conv How to encode the values while reading them.
     * @return The node's values as a string array.
     * @note Call GetType() to verify the node's data type to ensure that
     *      you are calling the correct @c GetValue___() function.
     */
    std::vector<wxSimpleJSON::Ptr_t> GetValueArrayObject() const;
    /**
     * @brief Returns the node's value as a double
     *      (if that is its value type).
     * @param defaultValue The value to return upon failure.
     * @note Call GetType() to verify the node's data type to ensure that
     *      you are calling the correct @c GetValue___() function.
     */
    double GetValueNumber(double defaultValue = -1) const;

    /**
     * @brief Returns a node's property (by name).
     * @param name The name of the property to get.
     * @return The property (as a node) if found. Call IsOk() to validate it.
     */
    wxSimpleJSON::Ptr_t GetProperty(const wxString &name) const;

    /**
     * @brief Deletes property with a given name,
     * @param name The name of the property to delete.
     * @return @c true if the property was successfully deleted.
     */
    bool DeleteProperty(const wxString &name);
    
    /**
     * @brief Deletes a property with a given zero-based array index.
     * @param idx The index of the property to delete.
     * @return @c true if the property was successfully deleted.
     */
    bool DeleteProperty(int idx);
    
     /**
     * @brief Check if node contains the specific Key.
       @details Returns @c false if the property cannot be found.
       @param name The name of the property to check for.
       @return @c true if the node contains the specified property.
     */
     bool HasProperty(const wxString& name);
     
    /**
     * @brief Get the value type that this node contains.
     * @returns The node's value type.
     */
     wxSimpleJSON::JSONType GetType();

    /**
     * @brief Gets Object Keys.
     * @param conv The (optional) encoding to read the keys with.
     * @return An array of strings containing object Keys.
     */
     wxArrayString GetObjectKeys(const wxMBConv &conv = wxConvUTF8);

    /**
     * @brief Converts this JSON object to string.
     * @details This is useful for debugging or displaying the JSON
     *      data to the client.
     * @param pretty @c true to apply indentations + spacing.
     * @param conv How to encode the string while writing it.
     * @return The JSON object as a string.
     */
    wxString Print(bool pretty = true, const wxMBConv &conv = wxConvUTF8) const;
};

#endif // WX_SIMPLE_JSON_H
