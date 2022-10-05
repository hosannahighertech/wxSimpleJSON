#include "wxSimpleJSON.h"
#include "cJSON/cJSON.h"

static cJSON *cJSONAllocNew()
{
    cJSON *node = (cJSON *)malloc(sizeof(cJSON));
    if(node) memset(node, 0, sizeof(cJSON));
    return node;
}

wxSimpleJSON::wxSimpleJSON()
    : m_d(NULL)
    , m_canDelete(false)
{
}

wxSimpleJSON::~wxSimpleJSON() {}

wxSimpleJSON::Ptr_t wxSimpleJSON::Create(wxSimpleJSON::JSONType type, bool isRoot)
{
    wxSimpleJSON *obj = new wxSimpleJSON();
    obj->m_d = cJSONAllocNew();
    obj->m_d->type = static_cast<int>(type);
    obj->m_canDelete = isRoot;
    wxSimpleJSON::Ptr_t ptr(obj, wxSimpleJSON::Destroy);
    return ptr;
}

wxSimpleJSON::Ptr_t wxSimpleJSON::Create(cJSON *p, bool canDelete)
{
    wxSimpleJSON *obj = new wxSimpleJSON();
    obj->m_d = p;
    obj->m_canDelete = canDelete;
    wxSimpleJSON::Ptr_t ptr(obj, wxSimpleJSON::Destroy);
    return ptr;
}

void wxSimpleJSON::Destroy(wxSimpleJSON *obj)
{
    if(obj->m_canDelete && obj->m_d) {
        cJSON_Delete(obj->m_d);
    }
    obj->m_d = nullptr;
    wxDELETE(obj);
}

wxSimpleJSON &wxSimpleJSON::ArrayAdd(wxSimpleJSON::Ptr_t obj)
{
    cJSON_AddItemToArray(m_d, obj->m_d);
    return *this;
}

wxSimpleJSON &wxSimpleJSON::ArrayAdd(const wxString &value, const wxMBConv &conv)
{
    cJSON_AddItemToArray(m_d, cJSON_CreateString(value.mb_str(conv).data()));
    return *this;
}

wxSimpleJSON &wxSimpleJSON::ArrayAdd(double value)
{
    cJSON_AddItemToArray(m_d, cJSON_CreateNumber(value));
    return *this;
}

wxSimpleJSON &wxSimpleJSON::Add(const wxString &name, wxSimpleJSON::Ptr_t obj)
{
    DeleteProperty(name);
    
    cJSON_AddItemToObject(m_d, name.mb_str(wxConvUTF8).data(), obj->m_d);
    return *this;
}

wxSimpleJSON &wxSimpleJSON::Add(const wxString &name, const wxString &value, const wxMBConv &conv)
{
    DeleteProperty(name);
    
    cJSON_AddStringToObject(m_d, name.mb_str(wxConvUTF8).data(), value.mb_str(conv).data());
    return *this;
}

wxSimpleJSON &wxSimpleJSON::Add(const wxString &name, double value)
{
    DeleteProperty(name);
    
    cJSON_AddNumberToObject(m_d, name.mb_str(wxConvUTF8).data(), value);
    return *this;
}

wxSimpleJSON &wxSimpleJSON::AddNull(const wxString &name)
{
    DeleteProperty(name);
    
    cJSON_AddNullToObject(m_d, name.mb_str(wxConvUTF8).data());
    return *this;
}

wxSimpleJSON &wxSimpleJSON::ArrayAdd(const wxArrayString &arr, const wxMBConv &conv)
{
    wxSimpleJSON::Ptr_t parr = Create(wxSimpleJSON::IS_ARRAY);
    for(size_t i = 0; i < arr.size(); ++i) {
        parr->ArrayAdd(arr.Item(i), conv);
    }
    return ArrayAdd(parr);
}

wxSimpleJSON &wxSimpleJSON::Add(const wxString &name, const wxArrayString &arr, const wxMBConv &conv)
{
    DeleteProperty(name);
    
    wxSimpleJSON::Ptr_t parr = Create(wxSimpleJSON::IS_ARRAY);
    for(size_t i = 0; i < arr.size(); ++i) {
        parr->ArrayAdd(arr.Item(i), conv);
    }
    return Add(name, parr);
}

size_t wxSimpleJSON::ArraySize() const { return cJSON_GetArraySize(m_d); }

wxSimpleJSON::Ptr_t wxSimpleJSON::Item(size_t index) const
{
    cJSON* item{ nullptr };
    if(!m_d || (m_d->type != cJSON_Array)) {
        return Create(item);
    }
    item = cJSON_GetArrayItem(m_d, index);
    return Create(item);
}

wxString wxSimpleJSON::GetValueString(const wxString &defaultValue,
                                      const wxMBConv &conv) const
{
    if(!m_d || (m_d->type != cJSON_String)) {
        return defaultValue;
    }
    return wxString(m_d->valuestring, conv);
}

wxArrayString wxSimpleJSON::GetValueArrayString(const wxMBConv &conv) const
{
    if(!m_d || (m_d->type != cJSON_Array)) {
        return wxArrayString();
    }

    wxArrayString arr;
    wxSimpleJSON::Ptr_t parr = Create(m_d);
    for(size_t i = 0; i < parr->ArraySize(); ++i) {
        arr.Add(parr->Item(i)->GetValueString(wxEmptyString, conv));
    }
    return arr;
}

#ifdef wxUSE_STL 
std::vector<wxString> wxSimpleJSON::GetValueStringVector(const wxMBConv& conv) const
{
    if (!m_d || (m_d->type != cJSON_Array)) {
        return std::vector<wxString>();
    }

    std::vector<wxString> arr;
    wxSimpleJSON::Ptr_t parr = Create(m_d);
    for (size_t i = 0; i < parr->ArraySize(); ++i) {
        arr.emplace_back(parr->Item(i)->GetValueString(wxEmptyString, conv));
    }
    return arr;
}

std::vector<wxSimpleJSON::Ptr_t> wxSimpleJSON::GetValueArrayObject() const
{
    if(!m_d || (m_d->type != cJSON_Array)) {
        return std::vector<wxSimpleJSON::Ptr_t>();
    }

    std::vector<wxSimpleJSON::Ptr_t> arr;
    wxSimpleJSON::Ptr_t parr = Create(m_d);
    for(size_t i = 0; i < parr->ArraySize(); ++i) {
        arr.push_back(parr->Item(i));
    }
    return arr;
}
#endif

double wxSimpleJSON::GetValueNumber(double defaultValue) const
{
    if(!m_d || (m_d->type != cJSON_Number)) {
        return defaultValue;
    }
    return m_d->valuedouble;
}

#ifdef wxUSE_STL 
std::vector<double> wxSimpleJSON::GetValueArrayNumber(double defaultValue) const
{
    if (!m_d || (m_d->type != cJSON_Array)) {
        return std::vector<double>();
    }

    std::vector<double> arr;
    wxSimpleJSON::Ptr_t parr = Create(m_d);
    for (size_t i = 0; i < parr->ArraySize(); ++i) {
        arr.emplace_back(parr->Item(i)->GetValueNumber(defaultValue));
    }
    return arr;
}
#endif

wxSimpleJSON::Ptr_t wxSimpleJSON::GetProperty(const wxString &name) const
{
    if(!m_d || (m_d->type != cJSON_Object)) {
        return Create(nullptr);
    }
    return Create(cJSON_GetObjectItem(m_d, name.mb_str(wxConvUTF8).data()));
}

wxSimpleJSON &wxSimpleJSON::ArrayAdd(bool value)
{
    if(!m_d || (m_d->type != cJSON_Array)) {
        return *this;
    }
    if(value) {
        cJSON_AddItemToArray(m_d, cJSON_CreateTrue());
    }
    else
    {
        cJSON_AddItemToArray(m_d, cJSON_CreateFalse());
    }
    return *this;
}

wxSimpleJSON &wxSimpleJSON::Add(const wxString &name, bool value)
{
    DeleteProperty(name);
    
    if(!m_d || (m_d->type != cJSON_Object)) {
        return *this;
    }
    cJSON_AddBoolToObject(m_d, name.mb_str(wxConvUTF8).data(), value);
    return *this;
}

bool wxSimpleJSON::GetValueBool(bool defaultValue) const
{
    if(!m_d || (m_d->type != cJSON_True && m_d->type != cJSON_False)) {
        return defaultValue;
    }
    // at this point, type can only be cJSON_True or cJSON_False
    return m_d->type == cJSON_True;
}

#ifdef wxUSE_STL 
std::vector<bool> wxSimpleJSON::GetValueArrayBool(bool defaultValue) const
{
    if (!m_d || (m_d->type != cJSON_Array)) {
        return std::vector<bool>();
    }

    std::vector<bool> arr;
    wxSimpleJSON::Ptr_t parr = Create(m_d);
    for (size_t i = 0; i < parr->ArraySize(); ++i) {
        arr.push_back(parr->Item(i)->GetValueBool(defaultValue));
    }
    return arr;
}
#endif

wxSimpleJSON::Ptr_t wxSimpleJSON::Create(const wxString &buffer, bool isRoot, const wxMBConv &conv)
{
    const char *parseEnd{ nullptr };
    const auto scopedBuffer = buffer.mb_str(conv);
    cJSON *p = cJSON_ParseWithOpts(scopedBuffer.data(), &parseEnd, true);
    auto parsedNode = Create(p, isRoot);
    if (p == nullptr) {
        const auto lineCount = std::count(scopedBuffer.data(), parseEnd, '\n');
        // get the full line where the error occurred
        auto startOfErrorLine = parseEnd;
        while (startOfErrorLine > scopedBuffer.data() && *startOfErrorLine != '\n') {
            --startOfErrorLine;
            if (*startOfErrorLine == '\n') {
                ++startOfErrorLine;
                break;
                }
            }
        auto endOfErrorLine = parseEnd;
        while (*endOfErrorLine != 0 && *endOfErrorLine != '\n') {
            ++endOfErrorLine;
            if (*endOfErrorLine == '\n') {
                --endOfErrorLine;
                break;
                }
            }
        // get the text where the error occurred
        wxString errorLine(startOfErrorLine, conv, endOfErrorLine-startOfErrorLine);
        wxString errorLineStartOfError(parseEnd, conv, endOfErrorLine-parseEnd);
        parsedNode->SetLastError(
            wxString::Format(_(L"JSON parsing error at line %s, column %s.\n\n"
                                "full line:\n%s\n\n"
                                "start of error:\n%s"),
                wxNumberFormatter::ToString(lineCount + 1 /* human readable 1 indexed*/, 0,
                                            wxNumberFormatter::Style::Style_WithThousandsSep),
                wxNumberFormatter::ToString((parseEnd-startOfErrorLine) + 1 /* human readable 1 indexed*/, 0,
                                            wxNumberFormatter::Style::Style_WithThousandsSep),
                errorLine, errorLineStartOfError));
    }
    return parsedNode;
}

wxSimpleJSON::Ptr_t wxSimpleJSON::LoadFile(const wxFileName &filename, const wxMBConv &conv)
{
    if(!filename.Exists()) {
        return Create(nullptr);
    }
    wxFFile fp(filename.GetFullPath(), "rb");
    wxString content;
    if(fp.IsOpened() && fp.ReadAll(&content, conv)) {
        fp.Close();
        return Create(content, true, conv);
    }
    return Create(nullptr);
}

bool wxSimpleJSON::Save(const wxFileName &filename, const wxMBConv &conv)
{
    wxFFile fp(filename.GetFullPath(), "wb");
    if(fp.IsOpened()) {
        fp.Write(Print(true, conv), conv);
        fp.Close();
        return true;
    }
    return false;
}

bool wxSimpleJSON::DeleteProperty(const wxString &name)
{
    if(!m_d || (m_d->type != cJSON_Object)) {
        return false;
    }
    cJSON *p = cJSON_GetObjectItem(m_d, name.mb_str(wxConvUTF8).data());
    if(!p) {
        return false;
    }
    cJSON_DeleteItemFromObject(m_d, name.mb_str(wxConvUTF8).data());
    return true;
}

bool wxSimpleJSON::DeleteProperty(int idx)
{
    if(!m_d || (m_d->type != cJSON_Array)) {
        return false;
    }
    cJSON_DeleteItemFromArray(m_d, idx);
    return true;
}

bool wxSimpleJSON::HasProperty(const wxString& name)
{
    if(!m_d || (m_d->type != cJSON_Object)) {
        return false;
    }
    cJSON *p = cJSON_GetObjectItem(m_d, name.mb_str(wxConvUTF8).data());
    if(!p) {
        return false;
    }
    return true;
}

wxSimpleJSON::JSONType wxSimpleJSON::GetType() const
{
    return (IsOk() ? static_cast<wxSimpleJSON::JSONType>(m_d->type) : JSONType::IS_INVALID);
}

wxArrayString wxSimpleJSON::GetObjectKeys(const wxMBConv &conv)
{
    cJSON *current_element = nullptr;
    char *current_key = nullptr;
    wxArrayString keys;

    cJSON_ArrayForEach(current_element, m_d)
    {
        current_key = current_element->string;
        if (current_key != nullptr)
        {
            keys.Add(wxString(current_key, conv));
        }
    }
    return keys;
}

wxString wxSimpleJSON::Print(bool pretty, const wxMBConv &conv) const
{
    char *b = pretty ? cJSON_Print(m_d) : cJSON_PrintUnformatted(m_d);
    wxString s(b, conv);
    free(b);
    return s;
}
