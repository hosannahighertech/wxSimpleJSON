#include <gtest/gtest.h>
#include "../src/wxSimpleJSON.h"

//-----------------------------------------------------
TEST(Types, String)
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"user-name": "Blake M."
})");
	ASSERT_TRUE(json->IsOk());
	auto userName = json->GetProperty(L"user-name");
	ASSERT_TRUE(userName->IsOk());
	EXPECT_FALSE(userName->IsValueArray());
	EXPECT_FALSE(userName->IsValueBoolean());
	EXPECT_FALSE(userName->IsValueNull());
	EXPECT_FALSE(userName->IsValueNumber());
	EXPECT_FALSE(userName->IsValueObject());
	EXPECT_TRUE(userName->IsValueString());

	EXPECT_TRUE(userName->AsString() == L"Blake M.");
}

//-----------------------------------------------------
TEST(Types, Integer)
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"user-name": "Blake M.",
"user-id": 84517
})");
	ASSERT_TRUE(json->IsOk());
	auto userId = json->GetProperty(L"user-id");
	ASSERT_TRUE(userId->IsOk());
	EXPECT_FALSE(userId->IsValueArray());
	EXPECT_FALSE(userId->IsValueBoolean());
	EXPECT_FALSE(userId->IsValueNull());
	EXPECT_TRUE(userId->IsValueNumber());
	EXPECT_FALSE(userId->IsValueObject());
	EXPECT_FALSE(userId->IsValueString());

	EXPECT_TRUE(userId->AsDouble() == 84517);
}

//-----------------------------------------------------
TEST(Types, Double)
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"user-name": "Blake M.",
"user-id": 84517,
"salary": 2200000.97
})");
	ASSERT_TRUE(json->IsOk());
	auto salary = json->GetProperty(L"salary");
	ASSERT_TRUE(salary->IsOk());
	EXPECT_FALSE(salary->IsValueArray());
	EXPECT_FALSE(salary->IsValueBoolean());
	EXPECT_FALSE(salary->IsValueNull());
	EXPECT_TRUE(salary->IsValueNumber());
	EXPECT_FALSE(salary->IsValueObject());
	EXPECT_FALSE(salary->IsValueString());

	EXPECT_NEAR(salary->AsDouble(), 2200000.97, 2e-2);
}

//-----------------------------------------------------
TEST(Types, Boolean)
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"user-name": "Blake M.",
"user-id": 84517,
"salary": 2200000.97,
"active" : true
})");
	ASSERT_TRUE(json->IsOk());
	auto active = json->GetProperty(L"active");
	ASSERT_TRUE(active->IsOk());
	EXPECT_FALSE(active->IsValueArray());
	EXPECT_TRUE(active->IsValueBoolean());
	EXPECT_FALSE(active->IsValueNull());
	EXPECT_FALSE(active->IsValueNumber());
	EXPECT_FALSE(active->IsValueObject());
	EXPECT_FALSE(active->IsValueString());

	EXPECT_TRUE(active->AsBool());
}

//-----------------------------------------------------
TEST(Types, MissingProperty)
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"user-name": "Blake M.",
"user-id": 84517,
"salary": 2200000.97,
"active" : true
})");
	ASSERT_TRUE(json->IsOk());
	// there is no "location" property
	auto location = json->GetProperty(L"location");
	EXPECT_FALSE(location->IsOk());
	EXPECT_TRUE(location->IsNull());
	EXPECT_FALSE(location->IsValueArray());
	EXPECT_FALSE(location->IsValueNumber());
	EXPECT_FALSE(location->IsValueBoolean());
	EXPECT_FALSE(location->IsValueNull());
	EXPECT_FALSE(location->IsValueNumber());
	EXPECT_FALSE(location->IsValueObject());
	EXPECT_FALSE(location->IsValueString());
	// safely return empty/default content
	EXPECT_TRUE(location->AsArrayString().empty());
	EXPECT_TRUE(location->AsStrings().empty());
	EXPECT_TRUE(location->AsDoubles().empty());
	EXPECT_TRUE(location->AsBools().empty());
	EXPECT_TRUE(location->AsString().empty());
	EXPECT_TRUE(location->AsStrings().empty());
	EXPECT_TRUE(location->AsDouble(-1) == -1);
	EXPECT_FALSE(location->AsBool(false));
}

//-----------------------------------------------------
TEST(Types, EmptyArray)
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"datasets": []
})");
	ASSERT_TRUE(json->IsOk());
	auto datasets = json->GetProperty(L"datasets");
	ASSERT_TRUE(datasets->IsOk());
	EXPECT_TRUE(datasets->IsValueArray());
	EXPECT_FALSE(datasets->IsValueBoolean());
	EXPECT_FALSE(datasets->IsValueNull());
	EXPECT_FALSE(datasets->IsValueNumber());
	EXPECT_FALSE(datasets->IsValueObject());
	EXPECT_FALSE(datasets->IsValueString());

	EXPECT_TRUE(datasets->AsStrings().empty());
}

//-----------------------------------------------------
TEST(Types, StringArray)
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"datasets": [ "Head Count", "Enrollment" ]
})");
	ASSERT_TRUE(json->IsOk());
	auto datasets = json->GetProperty(L"datasets");
	ASSERT_TRUE(datasets->IsOk());
	EXPECT_TRUE(datasets->IsValueArray());
	EXPECT_FALSE(datasets->IsValueBoolean());
	EXPECT_FALSE(datasets->IsValueNull());
	EXPECT_FALSE(datasets->IsValueNumber());
	EXPECT_FALSE(datasets->IsValueObject());
	EXPECT_FALSE(datasets->IsValueString());

	ASSERT_TRUE(datasets->AsStrings().size() == 2);
	EXPECT_TRUE(datasets->AsStrings()[0] == L"Head Count");
	EXPECT_TRUE(datasets->AsStrings()[1] == L"Enrollment");
}

//-----------------------------------------------------
TEST(Types, BooleanArray)
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"active": [ false, true, false ]
})");
	ASSERT_TRUE(json->IsOk());
	auto actives = json->GetProperty(L"active");
	ASSERT_TRUE(actives->IsOk());
	EXPECT_TRUE(actives->IsValueArray());
	EXPECT_FALSE(actives->IsValueBoolean());
	EXPECT_FALSE(actives->IsValueNull());
	EXPECT_FALSE(actives->IsValueNumber());
	EXPECT_FALSE(actives->IsValueObject());
	EXPECT_FALSE(actives->IsValueString());

	ASSERT_TRUE(actives->AsBools().size() == 3);
	EXPECT_FALSE(actives->AsBools()[0]);
	EXPECT_TRUE(actives->AsBools()[1]);
	EXPECT_FALSE(actives->AsBools()[2]);
}

//-----------------------------------------------------
TEST(Types, DoubleArray)
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"modes": [ 3.759, 189.842957, 0, 8 ]
})");
	ASSERT_TRUE(json->IsOk());
	auto modes = json->GetProperty(L"modes");
	ASSERT_TRUE(modes->IsOk());
	EXPECT_TRUE(modes->IsValueArray());
	EXPECT_FALSE(modes->IsValueBoolean());
	EXPECT_FALSE(modes->IsValueNull());
	EXPECT_FALSE(modes->IsValueNumber());
	EXPECT_FALSE(modes->IsValueObject());
	EXPECT_FALSE(modes->IsValueString());

	ASSERT_TRUE(modes->AsDoubles().size() == 4);
	// ensure full precision was read
	EXPECT_NEAR(modes->AsDoubles()[0], 3.759, 2e-3);
	EXPECT_NEAR(modes->AsDoubles()[1], 189.842957, 2e-6);
	// simple integral values
	EXPECT_TRUE(modes->AsDoubles()[2] == 0);
	EXPECT_TRUE(modes->AsDoubles()[3] == 8);
}

//-----------------------------------------------------
TEST(Types, NodeArray)
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"datasets": [
    {
      "name": "Head Count",
      "file-path": "Head Count.txt",
      "year": 2025
    },
    {
      "name": "Enrollment",
      "file-path": "Enrollment.txt",
      "year": 1987
    }
]
})");
	ASSERT_TRUE(json->IsOk());
	auto datasets = json->GetProperty(L"datasets");
	ASSERT_TRUE(datasets->IsOk());
	EXPECT_TRUE(datasets->IsValueArray());
	EXPECT_FALSE(datasets->IsValueBoolean());
	EXPECT_FALSE(datasets->IsValueNull());
	EXPECT_FALSE(datasets->IsValueNumber());
	EXPECT_FALSE(datasets->IsValueObject());
	EXPECT_FALSE(datasets->IsValueString());

	// non-sensical conversions should fail, returning to default value
	EXPECT_TRUE(datasets->AsDouble() == -1);
	EXPECT_TRUE(datasets->AsArrayString().empty());
	EXPECT_TRUE(datasets->AsString().empty());
	EXPECT_TRUE(datasets->AsDoubles().empty());
	EXPECT_FALSE(datasets->AsBool());

	auto nodes = datasets->AsNodes();
	ASSERT_TRUE(nodes.size() == 2);
	auto currentDataset = nodes[0];
	ASSERT_TRUE(currentDataset->IsOk());
	EXPECT_TRUE(currentDataset->HasProperty(L"name"));
	EXPECT_TRUE(currentDataset->GetProperty(L"name")->IsOk());
	EXPECT_TRUE(currentDataset->GetProperty(L"name")->AsString() == L"Head Count");

	EXPECT_TRUE(currentDataset->HasProperty(L"file-path"));
	EXPECT_TRUE(currentDataset->GetProperty(L"file-path")->IsOk());
	EXPECT_TRUE(currentDataset->GetProperty(L"file-path")->AsString() == L"Head Count.txt");

	EXPECT_TRUE(currentDataset->HasProperty(L"year"));
	EXPECT_TRUE(currentDataset->GetProperty(L"year")->IsOk());
	EXPECT_TRUE(currentDataset->GetProperty(L"year")->AsDouble() == 2025);

	// next node
	currentDataset = nodes[1];
	ASSERT_TRUE(currentDataset->IsOk());
	EXPECT_TRUE(currentDataset->HasProperty(L"name"));
	EXPECT_TRUE(currentDataset->GetProperty(L"name")->IsOk());
	EXPECT_TRUE(currentDataset->GetProperty(L"name")->AsString() == L"Enrollment");

	EXPECT_TRUE(currentDataset->HasProperty(L"file-path"));
	EXPECT_TRUE(currentDataset->GetProperty(L"file-path")->IsOk());
	EXPECT_TRUE(currentDataset->GetProperty(L"file-path")->AsString() == L"Enrollment.txt");

	EXPECT_TRUE(currentDataset->HasProperty(L"year"));
	EXPECT_TRUE(currentDataset->GetProperty(L"year")->IsOk());
	EXPECT_TRUE(currentDataset->GetProperty(L"year")->AsDouble() == 1987);

	// everything should have been parsed OK, so shouldn't have any error log
	EXPECT_TRUE(json->GetLastError().empty());
}

//-----------------------------------------------------
TEST(Types, NodeArrayDifferentTypes)
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"years": [ 1972, "1973", 1974 ]
})");
	ASSERT_TRUE(json->IsOk());
	auto years = json->GetProperty(L"years");
	ASSERT_TRUE(years->IsOk());
	EXPECT_TRUE(years->IsValueArray());
	EXPECT_FALSE(years->IsValueBoolean());
	EXPECT_FALSE(years->IsValueNull());
	EXPECT_FALSE(years->IsValueNumber());
	EXPECT_FALSE(years->IsValueObject());
	EXPECT_FALSE(years->IsValueString());

	EXPECT_TRUE(years->AsDouble() == -1);
	EXPECT_TRUE(years->AsArrayString().size() == 1);
	EXPECT_TRUE(years->AsString().empty());
	EXPECT_TRUE(years->AsDoubles().size() == 2);
	EXPECT_FALSE(years->AsBool());

	auto nodes = years->AsNodes();
	ASSERT_TRUE(nodes.size() == 3);
	auto currentYear = nodes[0];
	ASSERT_TRUE(currentYear->IsOk());
	EXPECT_TRUE(currentYear->IsValueNumber());
	EXPECT_TRUE(currentYear->AsDouble() == 1972);

	currentYear = nodes[1];
	ASSERT_TRUE(currentYear->IsOk());
	EXPECT_TRUE(currentYear->IsValueString());
	EXPECT_TRUE(currentYear->AsString() == L"1973");

	currentYear = nodes[2];
	ASSERT_TRUE(currentYear->IsOk());
	EXPECT_TRUE(currentYear->IsValueNumber());
	EXPECT_TRUE(currentYear->AsDouble() == 1974);

	// everything should have been parsed OK, so shouldn't have any error log
	EXPECT_TRUE(json->GetLastError().empty());
}

//-----------------------------------------------------
TEST(File, LoadAndSave)
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"datasets": [
    {
      "name": "Head Count",
      "file-path": "Head Count.txt",
      "year": 2025
    },
    {
      "name": "Enrollment",
      "file-path": "Enrollment.txt",
      "year": 1987
    }
]
})");
	ASSERT_TRUE(json->IsOk());
	ASSERT_TRUE(json->Save(wxFileName::GetTempDir() + L"/json.tmp"));

	const auto jsonFromFile = wxSimpleJSON::LoadFile(wxFileName::GetTempDir() + L"/json.tmp");
	ASSERT_TRUE(jsonFromFile->IsOk());
	auto datasets = jsonFromFile->GetProperty(L"datasets");
	ASSERT_TRUE(datasets->IsOk());
	EXPECT_TRUE(datasets->IsValueArray());
	EXPECT_FALSE(datasets->IsValueBoolean());
	EXPECT_FALSE(datasets->IsValueNull());
	EXPECT_FALSE(datasets->IsValueNumber());
	EXPECT_FALSE(datasets->IsValueObject());
	EXPECT_FALSE(datasets->IsValueString());

	// non-sensical conversions should fail, returning to default value
	EXPECT_TRUE(datasets->AsDouble() == -1);
	EXPECT_TRUE(datasets->AsArrayString().empty());
	EXPECT_TRUE(datasets->AsString().empty());
	EXPECT_TRUE(datasets->AsDoubles().empty());
	EXPECT_FALSE(datasets->AsBool());

	auto nodes = datasets->AsNodes();
	ASSERT_TRUE(nodes.size() == 2);
	auto currentDataset = nodes[0];
	ASSERT_TRUE(currentDataset->IsOk());
	EXPECT_TRUE(currentDataset->HasProperty(L"name"));
	EXPECT_TRUE(currentDataset->GetProperty(L"name")->IsOk());
	EXPECT_TRUE(currentDataset->GetProperty(L"name")->AsString() == L"Head Count");

	EXPECT_TRUE(currentDataset->HasProperty(L"file-path"));
	EXPECT_TRUE(currentDataset->GetProperty(L"file-path")->IsOk());
	EXPECT_TRUE(currentDataset->GetProperty(L"file-path")->AsString() == L"Head Count.txt");

	EXPECT_TRUE(currentDataset->HasProperty(L"year"));
	EXPECT_TRUE(currentDataset->GetProperty(L"year")->IsOk());
	EXPECT_TRUE(currentDataset->GetProperty(L"year")->AsDouble() == 2025);

	// next node
	currentDataset = nodes[1];
	ASSERT_TRUE(currentDataset->IsOk());
	EXPECT_TRUE(currentDataset->HasProperty(L"name"));
	EXPECT_TRUE(currentDataset->GetProperty(L"name")->IsOk());
	EXPECT_TRUE(currentDataset->GetProperty(L"name")->AsString() == L"Enrollment");

	EXPECT_TRUE(currentDataset->HasProperty(L"file-path"));
	EXPECT_TRUE(currentDataset->GetProperty(L"file-path")->IsOk());
	EXPECT_TRUE(currentDataset->GetProperty(L"file-path")->AsString() == L"Enrollment.txt");

	EXPECT_TRUE(currentDataset->HasProperty(L"year"));
	EXPECT_TRUE(currentDataset->GetProperty(L"year")->IsOk());
	EXPECT_TRUE(currentDataset->GetProperty(L"year")->AsDouble() == 1987);

	// everything should have been parsed OK, so shouldn't have any error log
	EXPECT_TRUE(json->GetLastError().empty());
}

//-----------------------------------------------------
TEST(File, BadLoad)
{
	if (!wxFileName::FileExists(wxFileName::GetTempDir() + L"/json_this_likely_is_missing.tmp"))
	{
		const auto jsonFromFile = wxSimpleJSON::LoadFile(wxFileName::GetTempDir() + L"/json_this_likely_is_missing.tmp");
		ASSERT_FALSE(jsonFromFile->IsOk());
		ASSERT_TRUE(jsonFromFile->IsNull());
	}
}

//-----------------------------------------------------
TEST(Fuzzing, Garbage)
{
	// a missing quote, should fail gracefully with an error log
	const auto json = wxSimpleJSON::Create(LR"(
{
"datasets": [ "Head Count", Enrollment" ]
})");
	ASSERT_FALSE(json->IsOk());
	EXPECT_FALSE(json->GetLastError().empty());
}

//-----------------------------------------------------
TEST(AddDelete, Delete)
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"user-name": "Blake M.",
"user-id": 84517,
"salary": 2200000.97,
"status" : [ "active", "remote" ]
})");
	ASSERT_TRUE(json->IsOk());
	// out of range or non-existent
	EXPECT_FALSE(json->DeleteProperty(-1));
	EXPECT_FALSE(json->DeleteProperty(10));
	EXPECT_FALSE(json->DeleteProperty(L"bogus"));
	EXPECT_FALSE(json->DeleteProperty(L""));

	EXPECT_TRUE(json->GetProperty(L"status")->IsOk());
	EXPECT_FALSE(json->GetProperty(L"status")->DeleteProperty(-10)); // out of range
	EXPECT_FALSE(json->GetProperty(L"status")->DeleteProperty(2)); // out of range
	EXPECT_TRUE(json->GetProperty(L"status")->DeleteProperty(0));
	ASSERT_TRUE(json->GetProperty(L"status")->AsStrings().size() == 1);
	ASSERT_TRUE(json->GetProperty(L"status")->AsStrings()[0] == L"remote");
}

//-----------------------------------------------------
TEST(AddDelete, Add)
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"user-name": "Blake M.",
"user-id": 84517,
"salary": 2200000.97,
"status" : [ "active", "remote" ]
})");

	ASSERT_TRUE(json->IsOk());
	json->Add(L"user-name", wxString{ L"Stefano" });
	auto userName = json->GetProperty(L"user-name");
	EXPECT_TRUE(userName->IsOk());
	EXPECT_TRUE(userName->AsString() == wxString{ L"Stefano" });

	// make it a number
	json->Add(L"user-name", 105756.0);
	userName = json->GetProperty(L"user-name");
	EXPECT_TRUE(userName->IsOk());
	EXPECT_TRUE(userName->AsDouble() == 105756.0);

	// make it a bool
	json->Add(L"user-name", true);
	userName = json->GetProperty(L"user-name");
	EXPECT_TRUE(userName->IsOk());
	EXPECT_TRUE(userName->AsBool());

	// make it an array of strings
	json->Add(L"user-name", wxArrayString{ L"Blake M.", L"Stefano" });
	userName = json->GetProperty(L"user-name");
	EXPECT_TRUE(userName->IsOk());
	ASSERT_TRUE(userName->AsStrings().size() == 2);
	EXPECT_TRUE(userName->AsStrings()[0] == L"Blake M.");
	EXPECT_TRUE(userName->AsStrings()[1] == L"Stefano");

	// add an entirely new node
	json->Add(L"location", wxString{ L"Ohio" });
	auto location = json->GetProperty(L"location");
	EXPECT_TRUE(location->IsOk());
	EXPECT_FALSE(location->IsNull());
	EXPECT_TRUE(location->AsString() == wxString{ L"Ohio" });
}
