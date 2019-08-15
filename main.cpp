#include "XmlParser.hpp"

using namespace ez;

int main( int argc , char* argv[] ) {

	try {

		XmlTag tag = XmlTag::Parse(" < tag key = val  key2=val2> data < tag2 key = val  key2=val2>inner data</tag2>extra< / tag > ");

		if (tag["key"] != "val")
			return 1;

		if (tag["key2"] != "val2")
			return 1;

		if (tag.GetData() != " data extra")
			return 1;


		XmlTag tag2 = tag.GetChild("tag2");
		if (tag2["key"] != "val")
			return 1;

		if (tag2["key2"] != "val2")
			return 1;

		if (tag2.GetData() != "inner data")
			return 1;


		XmlTag note = XmlTag::Parse("<animation name=bounce timing-function=\"ease-in\">"
			"Sample animation config"
			"<from left=0 top=11>%33</from>"
			"<to right=99 bottom=600>%66</to>"
			"</animation>");
		if (note.GetName() != "animation")
			return 1;

		if (note["name"] != "bounce")
			return 1;

		if (note["timing-function"] != "\"ease-in\"")
			return 1;

		XmlTag from = note.GetChild("from");

		if (from["left"] != "0")
			return 1;

		if (from["top"] != "11")
			return 1;

		if (from.GetData() != "%33")
			return 1;

		XmlTag to = note.GetChild("to");

		if (to["right"] != "99")
			return 1;

		if (to["bottom"] != "600")
			return 1;

		if (to.GetData() != "%66")
			return 1;

	}
	catch (std::exception& e) {
		std::cout << std::string(e.what()) << std::endl;
		return 1;
	}

	return 0;

}



