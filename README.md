# XmlParser
Single header , unofficial xml parser for easy to use compatible with C++11
```cpp
#include "XmlParser.hpp"

using namespace ez;

int main( int argc , char* argv[] ) {

	try {

		XmlTag tag = XmlTag::Parse(" < tag key = val  key2=val2>"
						" data "
						"< tag2 key = val  key2=val2>"
						  "inner data"
						"</tag2>extra"
					     "< / tag > ");

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

	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return 1;
	}

	return 0;

}
```



