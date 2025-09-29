#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "../src/wxSimpleJSON.h"

//-----------------------------------------------------
TEST_CASE("String", "[types]")
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"user-name": "Blake M."
})");
	REQUIRE(json->IsOk());
	auto userName = json->GetProperty(L"user-name");
	REQUIRE(userName->IsOk());
	CHECK_FALSE(userName->IsValueArray());
	CHECK_FALSE(userName->IsValueBoolean());
	CHECK_FALSE(userName->IsValueNull());
	CHECK_FALSE(userName->IsValueNumber());
	CHECK_FALSE(userName->IsValueObject());
	CHECK(userName->IsValueString());

	CHECK(userName->AsString() == L"Blake M.");
}

//-----------------------------------------------------
TEST_CASE("Integer", "[types]")
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"user-name": "Blake M.",
"user-id": 84517
})");
	REQUIRE(json->IsOk());
	auto userId = json->GetProperty(L"user-id");
	REQUIRE(userId->IsOk());
	CHECK_FALSE(userId->IsValueArray());
	CHECK_FALSE(userId->IsValueBoolean());
	CHECK_FALSE(userId->IsValueNull());
	CHECK(userId->IsValueNumber());
	CHECK_FALSE(userId->IsValueObject());
	CHECK_FALSE(userId->IsValueString());

	CHECK(userId->AsDouble() == 84517);
}

//-----------------------------------------------------
TEST_CASE("Double", "[types]")
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"user-name": "Blake M.",
"user-id": 84517,
"salary": 2200000.97
})");
	REQUIRE(json->IsOk());
	auto salary = json->GetProperty(L"salary");
	REQUIRE(salary->IsOk());
	CHECK_FALSE(salary->IsValueArray());
	CHECK_FALSE(salary->IsValueBoolean());
	CHECK_FALSE(salary->IsValueNull());
	CHECK(salary->IsValueNumber());
	CHECK_FALSE(salary->IsValueObject());
	CHECK_FALSE(salary->IsValueString());

	CHECK_THAT(salary->AsDouble(), Catch::Matchers::WithinAbs(2200000.97, 2e-2));
}

//-----------------------------------------------------
TEST_CASE("Boolean", "[types]")
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"user-name": "Blake M.",
"user-id": 84517,
"salary": 2200000.97,
"active" : true
})");
	REQUIRE(json->IsOk());
	auto active = json->GetProperty(L"active");
	REQUIRE(active->IsOk());
	CHECK_FALSE(active->IsValueArray());
	CHECK(active->IsValueBoolean());
	CHECK_FALSE(active->IsValueNull());
	CHECK_FALSE(active->IsValueNumber());
	CHECK_FALSE(active->IsValueObject());
	CHECK_FALSE(active->IsValueString());

	CHECK(active->AsBool());
}

//-----------------------------------------------------
TEST_CASE("Missing Property", "[types]")
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"user-name": "Blake M.",
"user-id": 84517,
"salary": 2200000.97,
"active" : true
})");
	REQUIRE(json->IsOk());
	// there is no "location" property
	auto location = json->GetProperty(L"location");
	CHECK_FALSE(location->IsOk());
	CHECK(location->IsNull());
	CHECK_FALSE(location->IsValueArray());
	CHECK_FALSE(location->IsValueNumber());
	CHECK_FALSE(location->IsValueBoolean());
	CHECK_FALSE(location->IsValueNull());
	CHECK_FALSE(location->IsValueNumber());
	CHECK_FALSE(location->IsValueObject());
	CHECK_FALSE(location->IsValueString());
	// safely return empty/default content
	CHECK(location->AsArrayString().empty());
	CHECK(location->AsStrings().empty());
	CHECK(location->AsDoubles().empty());
	CHECK(location->AsBools().empty());
	CHECK(location->AsString().empty());
	CHECK(location->AsStrings().empty());
	CHECK(location->AsDouble(-1) == -1);
	CHECK_FALSE(location->AsBool(false));
}

//-----------------------------------------------------
TEST_CASE("Empty Array", "[types]")
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"datasets": []
})");
	REQUIRE(json->IsOk());
	auto datasets = json->GetProperty(L"datasets");
	REQUIRE(datasets->IsOk());
	CHECK(datasets->IsValueArray());
	CHECK_FALSE(datasets->IsValueBoolean());
	CHECK_FALSE(datasets->IsValueNull());
	CHECK_FALSE(datasets->IsValueNumber());
	CHECK_FALSE(datasets->IsValueObject());
	CHECK_FALSE(datasets->IsValueString());

	CHECK(datasets->AsStrings().empty());
}

//-----------------------------------------------------
TEST_CASE("String Array", "[types]")
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"datasets": [ "Head Count", "Enrollment" ]
})");
	REQUIRE(json->IsOk());
	auto datasets = json->GetProperty(L"datasets");
	REQUIRE(datasets->IsOk());
	CHECK(datasets->IsValueArray());
	CHECK_FALSE(datasets->IsValueBoolean());
	CHECK_FALSE(datasets->IsValueNull());
	CHECK_FALSE(datasets->IsValueNumber());
	CHECK_FALSE(datasets->IsValueObject());
	CHECK_FALSE(datasets->IsValueString());

	REQUIRE(datasets->AsStrings().size() == 2);
	CHECK(datasets->AsStrings()[0] == L"Head Count");
	CHECK(datasets->AsStrings()[1] == L"Enrollment");
}

//-----------------------------------------------------
TEST_CASE("Boolean Array", "[types]")
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"active": [ false, true, false ]
})");
	REQUIRE(json->IsOk());
	auto actives = json->GetProperty(L"active");
	REQUIRE(actives->IsOk());
	CHECK(actives->IsValueArray());
	CHECK_FALSE(actives->IsValueBoolean());
	CHECK_FALSE(actives->IsValueNull());
	CHECK_FALSE(actives->IsValueNumber());
	CHECK_FALSE(actives->IsValueObject());
	CHECK_FALSE(actives->IsValueString());

	REQUIRE(actives->AsBools().size() == 3);
	CHECK_FALSE(actives->AsBools()[0]);
	CHECK(actives->AsBools()[1]);
	CHECK_FALSE(actives->AsBools()[2]);
}

//-----------------------------------------------------
TEST_CASE("Double Array", "[types]")
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"modes": [ 3.759, 189.842957, 0, 8 ]
})");
	REQUIRE(json->IsOk());
	auto modes = json->GetProperty(L"modes");
	REQUIRE(modes->IsOk());
	CHECK(modes->IsValueArray());
	CHECK_FALSE(modes->IsValueBoolean());
	CHECK_FALSE(modes->IsValueNull());
	CHECK_FALSE(modes->IsValueNumber());
	CHECK_FALSE(modes->IsValueObject());
	CHECK_FALSE(modes->IsValueString());

	REQUIRE(modes->AsDoubles().size() == 4);
	// ensure full precision was read
	CHECK_THAT(modes->AsDoubles()[0], Catch::Matchers::WithinAbs(3.759, 2e-3));
	CHECK_THAT(modes->AsDoubles()[1], Catch::Matchers::WithinAbs(189.842957, 2e-6));
	// simple integral values
	CHECK(modes->AsDoubles()[2] == 0);
	CHECK(modes->AsDoubles()[3] == 8);
}

//-----------------------------------------------------
TEST_CASE("Node Array", "[types]")
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
	REQUIRE(json->IsOk());
	auto datasets = json->GetProperty(L"datasets");
	REQUIRE(datasets->IsOk());
	CHECK(datasets->IsValueArray());
	CHECK_FALSE(datasets->IsValueBoolean());
	CHECK_FALSE(datasets->IsValueNull());
	CHECK_FALSE(datasets->IsValueNumber());
	CHECK_FALSE(datasets->IsValueObject());
	CHECK_FALSE(datasets->IsValueString());

	// non-sensical conversions should fail, returning to default value
	CHECK(datasets->AsDouble() == -1);
	CHECK(datasets->AsArrayString().empty());
	CHECK(datasets->AsString().empty());
	CHECK(datasets->AsDoubles().empty());
    CHECK_FALSE(datasets->AsBool());

	auto nodes = datasets->AsNodes();
	REQUIRE(nodes.size() == 2);
	auto currentDataset = nodes[0];
	REQUIRE(currentDataset->IsOk());
	CHECK(currentDataset->HasProperty(L"name"));
	CHECK(currentDataset->GetProperty(L"name")->IsOk());
	CHECK(currentDataset->GetProperty(L"name")->AsString() == L"Head Count");

	CHECK(currentDataset->HasProperty(L"file-path"));
	CHECK(currentDataset->GetProperty(L"file-path")->IsOk());
	CHECK(currentDataset->GetProperty(L"file-path")->AsString() == L"Head Count.txt");

	CHECK(currentDataset->HasProperty(L"year"));
	CHECK(currentDataset->GetProperty(L"year")->IsOk());
	CHECK(currentDataset->GetProperty(L"year")->AsDouble() == 2025);

	// next node
	currentDataset = nodes[1];
	REQUIRE(currentDataset->IsOk());
	CHECK(currentDataset->HasProperty(L"name"));
	CHECK(currentDataset->GetProperty(L"name")->IsOk());
	CHECK(currentDataset->GetProperty(L"name")->AsString() == L"Enrollment");

	CHECK(currentDataset->HasProperty(L"file-path"));
	CHECK(currentDataset->GetProperty(L"file-path")->IsOk());
	CHECK(currentDataset->GetProperty(L"file-path")->AsString() == L"Enrollment.txt");

	CHECK(currentDataset->HasProperty(L"year"));
	CHECK(currentDataset->GetProperty(L"year")->IsOk());
	CHECK(currentDataset->GetProperty(L"year")->AsDouble() == 1987);

	// everything should have been parsed OK, so shouldn't have any error log
	CHECK(json->GetLastError().empty());
	}

//-----------------------------------------------------
TEST_CASE("Node Array Different Types", "[types]")
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"years": [ 1972, "1973", 1974 ]
})");
	REQUIRE(json->IsOk());
	auto years = json->GetProperty(L"years");
	REQUIRE(years->IsOk());
	CHECK(years->IsValueArray());
	CHECK_FALSE(years->IsValueBoolean());
	CHECK_FALSE(years->IsValueNull());
	CHECK_FALSE(years->IsValueNumber());
	CHECK_FALSE(years->IsValueObject());
	CHECK_FALSE(years->IsValueString());

	CHECK(years->AsDouble() == -1);
	CHECK(years->AsArrayString().size() == 1);
	CHECK(years->AsString().empty());
	CHECK(years->AsDoubles().size() == 2);
	CHECK_FALSE(years->AsBool());

	auto nodes = years->AsNodes();
	REQUIRE(nodes.size() == 3);
	auto currentYear = nodes[0];
	REQUIRE(currentYear->IsOk());
	CHECK(currentYear->IsValueNumber());
	CHECK(currentYear->AsDouble() == 1972);

	currentYear = nodes[1];
	REQUIRE(currentYear->IsOk());
	CHECK(currentYear->IsValueString());
	CHECK(currentYear->AsString() == L"1973");

	currentYear = nodes[2];
	REQUIRE(currentYear->IsOk());
	CHECK(currentYear->IsValueNumber());
	CHECK(currentYear->AsDouble() == 1974);

	// everything should have been parsed OK, so shouldn't have any error log
	CHECK(json->GetLastError().empty());
}

//-----------------------------------------------------
TEST_CASE("Load & Save", "[file]")
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
	REQUIRE(json->IsOk());
	REQUIRE(json->Save(wxFileName::GetTempDir() + L"/json.tmp"));

	const auto jsonFromFile = wxSimpleJSON::LoadFile(wxFileName::GetTempDir() + L"/json.tmp");
	REQUIRE(jsonFromFile->IsOk());
	auto datasets = jsonFromFile->GetProperty(L"datasets");
	REQUIRE(datasets->IsOk());
	CHECK(datasets->IsValueArray());
	CHECK_FALSE(datasets->IsValueBoolean());
	CHECK_FALSE(datasets->IsValueNull());
	CHECK_FALSE(datasets->IsValueNumber());
	CHECK_FALSE(datasets->IsValueObject());
	CHECK_FALSE(datasets->IsValueString());

	// non-sensical conversions should fail, returning to default value
	CHECK(datasets->AsDouble() == -1);
	CHECK(datasets->AsArrayString().empty());
	CHECK(datasets->AsString().empty());
	CHECK(datasets->AsDoubles().empty());
	CHECK_FALSE(datasets->AsBool());

	auto blah = datasets->AsDoubles();

	auto nodes = datasets->AsNodes();
	REQUIRE(nodes.size() == 2);
	auto currentDataset = nodes[0];
	REQUIRE(currentDataset->IsOk());
	CHECK(currentDataset->HasProperty(L"name"));
	CHECK(currentDataset->GetProperty(L"name")->IsOk());
	CHECK(currentDataset->GetProperty(L"name")->AsString() == L"Head Count");

	CHECK(currentDataset->HasProperty(L"file-path"));
	CHECK(currentDataset->GetProperty(L"file-path")->IsOk());
	CHECK(currentDataset->GetProperty(L"file-path")->AsString() == L"Head Count.txt");

	CHECK(currentDataset->HasProperty(L"year"));
	CHECK(currentDataset->GetProperty(L"year")->IsOk());
	CHECK(currentDataset->GetProperty(L"year")->AsDouble() == 2025);

	// next node
	currentDataset = nodes[1];
	REQUIRE(currentDataset->IsOk());
	CHECK(currentDataset->HasProperty(L"name"));
	CHECK(currentDataset->GetProperty(L"name")->IsOk());
	CHECK(currentDataset->GetProperty(L"name")->AsString() == L"Enrollment");

	CHECK(currentDataset->HasProperty(L"file-path"));
	CHECK(currentDataset->GetProperty(L"file-path")->IsOk());
	CHECK(currentDataset->GetProperty(L"file-path")->AsString() == L"Enrollment.txt");

	CHECK(currentDataset->HasProperty(L"year"));
	CHECK(currentDataset->GetProperty(L"year")->IsOk());
	CHECK(currentDataset->GetProperty(L"year")->AsDouble() == 1987);

	// everything should have been parsed OK, so shouldn't have any error log
	CHECK(json->GetLastError().empty());
}

//-----------------------------------------------------
TEST_CASE("Bad Load", "[file]")
{
	if (!wxFileName::FileExists(wxFileName::GetTempDir() + L"/json_this_likely_is_missing.tmp"))
	{
		const auto jsonFromFile = wxSimpleJSON::LoadFile(wxFileName::GetTempDir() + L"/json_this_likely_is_missing.tmp");
		REQUIRE_FALSE(jsonFromFile->IsOk());
		REQUIRE(jsonFromFile->IsNull());
	}
}

//-----------------------------------------------------
TEST_CASE("Garbage", "[fuzzing]")
{
	// a missing quote, should fail gracefully with an error log
	const auto json = wxSimpleJSON::Create(LR"(
{
"datasets": [ "Head Count", Enrollment" ]
})");
	REQUIRE_FALSE(json->IsOk());
	CHECK_FALSE(json->GetLastError().empty());
}

//-----------------------------------------------------
TEST_CASE("Delete", "[add/delete]")
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"user-name": "Blake M.",
"user-id": 84517,
"salary": 2200000.97,
"status" : [ "active", "remote" ]
})");
	REQUIRE(json->IsOk());
	// out of range or non-existent
	CHECK_FALSE(json->DeleteProperty(-1));
	CHECK_FALSE(json->DeleteProperty(10));
	CHECK_FALSE(json->DeleteProperty(L"bogus"));
	CHECK_FALSE(json->DeleteProperty(L""));

	CHECK(json->GetProperty(L"status")->IsOk());
	CHECK_FALSE(json->GetProperty(L"status")->DeleteProperty(-10)); // out of range
	CHECK_FALSE(json->GetProperty(L"status")->DeleteProperty(2)); // out of range
	CHECK(json->GetProperty(L"status")->DeleteProperty(0));
	REQUIRE(json->GetProperty(L"status")->AsStrings().size() == 1);
	REQUIRE(json->GetProperty(L"status")->AsStrings()[0] == L"remote");
}

//-----------------------------------------------------
TEST_CASE("Add", "[add/delete]")
{
	const auto json = wxSimpleJSON::Create(LR"(
{
"user-name": "Blake M.",
"user-id": 84517,
"salary": 2200000.97,
"status" : [ "active", "remote" ]
})");

	auto blah = json->Print();

	REQUIRE(json->IsOk());
	json->Add(L"user-name", wxString{ L"Stefano" });
	auto userName = json->GetProperty(L"user-name");
	CHECK(userName->IsOk());
	CHECK(userName->AsString() == wxString{ L"Stefano" });

	// make it a number
	json->Add(L"user-name", 105756.0);
	userName = json->GetProperty(L"user-name");
	CHECK(userName->IsOk());
	CHECK(userName->AsDouble() == 105756.0);

	// make it a bool
	json->Add(L"user-name", true);
	userName = json->GetProperty(L"user-name");
	CHECK(userName->IsOk());
	CHECK(userName->AsBool());

	// make it an array of strings
	json->Add(L"user-name", wxArrayString{ L"Blake M.", L"Stefano" });
	userName = json->GetProperty(L"user-name");
	CHECK(userName->IsOk());
	REQUIRE(userName->AsStrings().size() == 2);
	CHECK(userName->AsStrings()[0] == L"Blake M.");
	CHECK(userName->AsStrings()[1] == L"Stefano");

	// add an entirely new node
	json->Add(L"location", wxString{ L"Ohio" });
	auto location = json->GetProperty(L"location");
	CHECK(location->IsOk());
	CHECK_FALSE(location->IsNull());
	CHECK(location->AsString() == wxString{ L"Ohio" });
}