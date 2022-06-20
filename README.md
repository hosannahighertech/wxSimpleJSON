wxSimpleJSON is a wxWidgets wrapper around CJSON. It helps a lot with JSON operations. wxSimpleJSON was initially created by Eran Ifrah, but since have been tweaked, bug fixed and few other lovely things. 

### Usage
To use it, simply include the files `src/cJSON/cJSON.c` and `src/wxSimpleJSON.cpp` and you are set.

### Examples
With the following example JSON file:

```
{
    "name": "Awesome 4K",
    "resolutions": [
        {
            "width": 1280,
            "height": 720
        },
        {
            "width": 1920,
            "height": 1080
        },
        {
            "width": 3840,
            "height": 2160
        }
    ]
}
```

You can load and parse it as follows:

```cpp
// load a JSON file
auto json = wxSimpleJSON::LoadFile(wxFileName(L"c:/users/stefano/monitor.json"));

// read the root-level name property
const auto monitorName = json->GetProperty("name");
// the following will be "Awesome 4K":
// monitorName->GetValueString();

// read the monitor's resolution information
// (which is another root-level property)
auto resolutionsInfo = json->GetProperty("resolutions");
if (resolutionsInfo->IsOk())
    {
    // get the array of resolutions and iterate through them
    auto resolutions = resolutionsInfo->GetValueArrayObject();
    for (const auto& resolution : resolutions)
        {
        // read the resolutions' dimensions
        auto width = resolution->GetProperty("width")->GetValueNumber();
        auto height = resolution->GetProperty("height")->GetValueNumber();
        }
    }
```

You can also make edits to the JSON file and save it:

```cpp
// load a JSON file
const auto json = wxSimpleJSON::LoadFile(wxFileName(L"c:/users/stefano/monitor.json"));

// read the monitor's resolution information
// (which is another root-level property)
auto resolutionsInfo = json->GetProperty("resolutions");
if (resolutionsInfo->IsOk())
    {
    // get the array of resolutions and iterate through them
    auto resolutions = resolutionsInfo->GetValueArrayObject();
    for (const auto& resolution : resolutions)
        {
        // read the resolutions' dimensions and add a comment
        // to the lower resolutions
        auto widthProperty = resolution->GetProperty("width");
        if (widthProperty->IsOk())
            {
            const auto width = widthProperty->GetValueNumber();
            if (width < 3840)
                {
                resolution->Add("instructions",
                                wxString("May require an adaptor"));
                }
            }
        }
    // save the updates
    json->Save(wxFileName(L"c:/users/stefano/monitor.json"));
    }
```

### License
The Library is released under wxWidgets License

### Reporting Bugs/ Requesting features
The Library seems to have pretty much all covered. But if you find any missing feature, or simply unwanted feature (aka bug) please let us know on issue tracker.

### We neeed Help
Do you have a little bit time? Help us with the following
- Writing unit tests for the library. We prefer [Catch](https://github.com/catchorg/Catch2)
- Help us write CMake to make the sources build as Shared/Static library across all platform

Hope you enjoy it
