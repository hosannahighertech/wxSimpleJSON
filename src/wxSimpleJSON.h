#ifndef WX_SIMPLE_JSON_H
#define WX_SIMPLE_JSON_H

#include <wx/sharedptr.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/filename.h>
#include "api.h"

typedef struct cJSON cJSON;

class API_EXPORT wxSimpleJSON
{
  protected:
    cJSON *m_d;
    bool m_canDelete;

  public:
// it matches the definitions in cJSON.h
/* cJSON Types: */
#define cJSON_Invalid (0)
#define cJSON_False (1 << 0)
#define cJSON_True (1 << 1)
#define cJSON_NULL (1 << 2)
#define cJSON_Number (1 << 3)
#define cJSON_String (1 << 4)
#define cJSON_Array (1 << 5)
#define cJSON_Object (1 << 6)
#define cJSON_Raw (1 << 7) /* raw json */

    enum eType
    {
        kInvalid = 0,
        kFalse = 1,
        kTrue = 2,
        kNull = 4,
        kNumber = 8,
        kString = 16,
        kArray = 32,
        kObject = 64,
        kRaw = 128
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
    static wxSimpleJSON::Ptr_t Create(wxSimpleJSON::eType type, bool isRoot = false);

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
     * @brief convert this JSON object to string
     * @param pretty apply indentations + spacing
     * @param conv string conversion object
     * @return JSON object as string
     */
    wxString Print(bool pretty = true, const wxMBConv &conv = wxConvUTF8) const;
};

#endif // WX_SIMPLE_JSON_H
