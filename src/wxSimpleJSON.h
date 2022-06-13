#ifndef WX_SIMPLE_JSON_H
#define WX_SIMPLE_JSON_H

#include <wx/sharedptr.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/filename.h>

#include <wx/dlimpexp.h>

#ifdef API_CREATING_DLL
#define API_EXPORT WXEXPORT
#else
#define API_EXPORT WXIMPORT
#endif

typedef struct cJSON cJSON;

class API_EXPORT wxSimpleJSON
{
  protected:
    cJSON *m_d;
    bool m_canDelete;

  public:

    enum JSONType
    {
        IS_INVALID = 0,
        IS_FALSE = 1,
        IS_TRUE = 2,
        IS_NULL = 4,
        IS_NUMBER = 8,
        IS_STRING = 16,
        IS_ARRAY = 32,
        IS_OBJECT = 64,
        IS_RAW = 128
    };
    virtual ~wxSimpleJSON();
    typedef wxSharedPtr<wxSimpleJSON> Ptr_t;

  private:
    // Constructor is private. The way to create an object is by using the ::Create method
    wxSimpleJSON();

    /**
     * @brief the deleter pointer
     */
    static void Destroy(wxSimpleJSON *obj);
    static wxSimpleJSON::Ptr_t Create(cJSON *p, bool canDelete = false);

  public:
    // Custom object generators
    /**
     * @brief create a new JSON node.
     * @param type the node type
     * @param isRoot pass this when creating the top level item (the root item of the JSON)
     * @note Check for IsNull()
     */
    static wxSimpleJSON::Ptr_t Create(wxSimpleJSON::JSONType, bool isRoot = false);

    /**
     * @brief parse and reutrn wxSimpleJSON object
     * @note Check for IsNull()
     */
    static wxSimpleJSON::Ptr_t Create(const wxString &buffer, bool isRoot = false, const wxMBConv &conv = wxConvUTF8);

    /**
     * @brief read JSON content from a file, parse and reutrn wxSimpleJSON object
     * @note Check for IsNull()
     */
    static wxSimpleJSON::Ptr_t Create(const wxFileName &filename, const wxMBConv &conv = wxConvUTF8);

    /**
     * @brief save the content of this object to a file
     */
    bool Save(const wxFileName &filename, const wxMBConv &conv = wxConvUTF8);

    /**
     * @brief is this object NULL?
     */
    inline bool IsNull() const { return (m_d == NULL); }

    // Array manipulation
    wxSimpleJSON &ArrayAdd(wxSimpleJSON::Ptr_t obj);
    wxSimpleJSON &ArrayAdd(const wxString &value, const wxMBConv &conv = wxConvUTF8);
    wxSimpleJSON &ArrayAdd(double value);
    wxSimpleJSON &ArrayAdd(bool value);
    wxSimpleJSON &ArrayAdd(const wxArrayString &arr, const wxMBConv &conv = wxConvUTF8);

    /**
     * @brief return the array size. Returns 0 if this object is not an array
     */
    size_t ArraySize() const;
    /**
     * @brief return an array item at a given position
     * @param index item position in the array
     * @return valid wxSimpleJSON::Ptr_t obj is a Null object (call wxSimpleJSON::IsNull() to test for null)
     */
    wxSimpleJSON::Ptr_t Item(size_t index) const;

    // Object manipulation
    wxSimpleJSON &Add(const wxString &name, wxSimpleJSON::Ptr_t obj);
    wxSimpleJSON &Add(const wxString &name, const wxString &value, const wxMBConv &conv = wxConvUTF8);
    wxSimpleJSON &Add(const wxString &name, double value);
    wxSimpleJSON &Add(const wxString &name, bool value);
    wxSimpleJSON &AddNull(const wxString &name);
    wxSimpleJSON &Add(const wxString &name, const wxArrayString &arr, const wxMBConv &conv = wxConvUTF8);

    /**
     * @brief return the string value
     */
    wxString GetValueString(const wxMBConv &conv = wxConvUTF8) const;

    /**
     * @brief return the string value
     */
    bool GetValueBool(bool defaultValue = false) const;

    /**
     * @brief return the array string value
     */
    wxArrayString GetValueArrayString(const wxMBConv &conv = wxConvUTF8) const;
    /**
     * @brief return the array Object value
     */
    std::vector<wxSimpleJSON::Ptr_t> GetValueArrayObject() const;
    /**
     * @brief return the number value
     */
    double GetValueNumber(double defaultValue = -1) const;

    /**
     * @brief return an object property
     */
    wxSimpleJSON::Ptr_t GetProperty(const wxString &name) const;

    /**
     * @brief delete property with a given name
     */
    bool DeleteProperty(const wxString &name);
    
    /**
     * @brief delete property with a given zero based array index
     */
    bool DeleteProperty(int idx);
    
     /**
     * @brief Check if object contains the specific Key. On non object always returns false
     */
     bool HasProperty(const wxString& name);
     
    /**
     * @brief Get the Type of this Node
     */
     wxSimpleJSON::JSONType GetType();

    /**
     * @brief Get Object Keys
     * @return array of Strings containing object Keys
     */
     wxArrayString GetObjectKeys(const wxMBConv &conv = wxConvUTF8);

    /**
     * @brief convert this JSON object to string
     * @param pretty apply indentations + spacing
     * @param conv string conversion object
     * @return JSON object as string
     */
    wxString Print(bool pretty = true, const wxMBConv &conv = wxConvUTF8) const;
};

#endif // WX_SIMPLE_JSON_H
