#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <algorithm>
#include <unordered_map>
#include <string>

namespace ez {
	class Util {
	public:
		static std::int64_t IsFollowedBy(std::string::const_iterator it, std::string::const_iterator end, const std::string& followed, const std::string& skip) {

			auto tmp = it;

			for (int i = 0; it != end && i < std::size(followed); ++it) {
				if (std::find(std::begin(skip), std::end(skip), *it) != std::end(skip)) continue; //if it is skip char go with next one
				if (*it != followed[i]) return 0;

				i++;

			}

			return std::distance(tmp, it);

		}


	};

	class XmlTag
	{
	private:
		std::string name;
		std::string data;
		std::vector< std::pair< std::string, std::string > > attrs;
		std::vector<XmlTag> kids;
		std::string footer;

		enum cursor {
			START,
			HEADER_TAG,
			HEADER_KEY,
			HEADER_VAL,
			DATA,
			FOOTER,
			END
		}cur;
	public:
		XmlTag() :cur(START) {

		};


		std::string operator[](const std::string& attr_name) {

			auto it = std::find_if(attrs.begin(), attrs.end(), [&attr_name](auto attr) {
				return attr_name == attr.first;
			});

			if (it == attrs.end())
				throw std::exception("Attr Not Found");

			return it->second;
		}

		std::string GetName() const {
			return name;
		}


		XmlTag GetChild(const XmlTag& parent, const std::string& name) const {
			auto it = std::find_if(parent.kids.begin(), parent.kids.end(), [&name](auto& kid) {
				return kid.name == name;
			});

			if (it == parent.kids.end())
				throw std::exception("Child Not Found");

			return *it;
		}

		XmlTag GetChild(const std::string& name) const {
			return GetChild(*this, name);
		}

		void AppendKeyVal(std::string&& key, std::string&& val) {
			this->attrs.emplace_back(std::pair<std::string, std::string>(std::move(key), std::move(val)));
		}


		static XmlTag Parse(const std::string& string) {
			return Parse(string.begin(), string.end());
		}

		std::string GetData() const {
			return this->data;
		}

	private:
		static XmlTag Parse(std::string::const_iterator& it, std::string::const_iterator& end) {

			XmlTag tag;

			std::string key;
			std::string val;
			std::int64_t diff = 0;


			while (it != end) {


				switch (tag.cur)
				{
				case START:
					if (*it != '<')
						break;
					tag.cur = HEADER_TAG;
					break;

				case HEADER_TAG:
					diff = Util::IsFollowedBy(it, end, "/>", " ");
					if (diff > 0) {
						std::advance(it, diff);
						tag.cur = END;
						continue;
					}

					if (*it == ' ') {
						if (tag.name.length() > 0) {
							tag.cur = HEADER_KEY;
							break;
						}
						break;
					}
					else if (*it == '>') {
						tag.cur = HEADER_VAL;
						continue;
					}


					tag.name.push_back(*it);
					break;

				case HEADER_KEY:
					diff = Util::IsFollowedBy(it, end, "/>", " ");
					if (diff > 0) {
						std::advance(it, diff);
						if (key.length() > 0) {
							tag.AppendKeyVal(std::move(key), std::move(val));
						}
						tag.cur = END;
						continue;
					}


					if (*it == '=') {
						tag.cur = HEADER_VAL;
						break;
					}
					else if (*it == ' ') {
						if (key.length() > 0) {
							diff = Util::IsFollowedBy(it, end, "=", " ");
							if (diff == 0) {
								tag.AppendKeyVal(std::move(key), std::move(val));
							}
						}
						break;
					}

					key.push_back(*it);

					break;

				case HEADER_VAL:
					diff = Util::IsFollowedBy(it, end, "/>", " ");
					if (diff > 0) { // ">" , "/>"
						std::advance(it, diff);
						if (key.length() > 0) {
							tag.AppendKeyVal(std::move(key), std::move(val));
						}

						tag.cur = END;
						continue;

					}

					if (*it == '>') {

						if (key.length() > 0) {
							tag.AppendKeyVal(std::move(key), std::move(val));
						}
						tag.cur = DATA;
						break;
					}
					else if (*it == ' ') {
						if (val.length() > 0) {
							tag.AppendKeyVal(std::move(key), std::move(val));
							tag.cur = HEADER_KEY;
						}
						break;
					}

					val.push_back(*it);

					break;

				case DATA:
					if (*it == '<') {
						diff = Util::IsFollowedBy(it, end, "</", " ");
						if (diff > 0) {
							std::advance(it, diff);
							tag.cur = FOOTER;
							continue;
						}

						XmlTag kid = Parse(it, end);
						tag.kids.push_back(std::move(kid));
						continue;
					}

					if (*it == ' ') {
						//if (tag.data.length() == 0)
						//	break;
					}

					tag.data.push_back(*it);
					break;

				case FOOTER:

					if (*it == '>') {
						tag.cur = END;
						break;
					}

					if (*it != ' ') {
						tag.footer.push_back(*it);
					}

					break;
				case END:
					return tag;
				default:
					break;
				}


				it++;

			}

			if (tag.footer.length() > 0) {
				if (tag.footer != tag.name)
					throw std::exception("Malformed xml");
			}

			return tag;
		}



	};

};

