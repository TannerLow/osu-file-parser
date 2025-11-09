#include <OsuFileParser.h>
#include <iostream>

int main() {
	std::cout << ofp::testOFP("hello world") << std::endl;
	// C:\Users\Baio\Desktop\Games\osu!\Songs\1061136 osu!mania 7K Dan Course - Dan Phase IV\osu!mania 7K Dan Course - Dan Phase IV (Jinjin) [Stellium Dan (Regular)].osu
	// C:\Users\Baio\Desktop\Games\osu!\Songs\985208 Co shu Nie - Zettai Zetsumei\Co shu Nie - Zettai Zetsumei (sankansuki) [Zetsubou].osu
	ofp::readFile("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\1061136 osu!mania 7K Dan Course - Dan Phase IV\\osu!mania 7K Dan Course - Dan Phase IV (Jinjin) [Stellium Dan (Regular)].osu");
	ofp::readFile("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\985208 Co shu Nie - Zettai Zetsumei\\Co shu Nie - Zettai Zetsumei (sankansuki) [Zetsubou].osu");

	std::cout << "=== [read lines 1] ===" << std::endl;

	ofp::readLines1("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\1061136 osu!mania 7K Dan Course - Dan Phase IV\\osu!mania 7K Dan Course - Dan Phase IV (Jinjin) [Stellium Dan (Regular)].osu");
	ofp::readLines1("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\985208 Co shu Nie - Zettai Zetsumei\\Co shu Nie - Zettai Zetsumei (sankansuki) [Zetsubou].osu");

	std::cout << "=== [read lines 2] ===" << std::endl;

	ofp::readLines2("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\1061136 osu!mania 7K Dan Course - Dan Phase IV\\osu!mania 7K Dan Course - Dan Phase IV (Jinjin) [Stellium Dan (Regular)].osu");
	ofp::readLines2("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\985208 Co shu Nie - Zettai Zetsumei\\Co shu Nie - Zettai Zetsumei (sankansuki) [Zetsubou].osu");

	std::cout << "=== [read lines 3] ===" << std::endl;

	ofp::readLines3("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\1061136 osu!mania 7K Dan Course - Dan Phase IV\\osu!mania 7K Dan Course - Dan Phase IV (Jinjin) [Stellium Dan (Regular)].osu");
	ofp::readLines3("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\985208 Co shu Nie - Zettai Zetsumei\\Co shu Nie - Zettai Zetsumei (sankansuki) [Zetsubou].osu");
	
	std::cout << "=== [read osu file] ===" << std::endl;
	
	ofp::readOsuFile("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\1061136 osu!mania 7K Dan Course - Dan Phase IV\\osu!mania 7K Dan Course - Dan Phase IV (Jinjin) [Stellium Dan (Regular)].osu");
	ofp::readOsuFile("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs\\985208 Co shu Nie - Zettai Zetsumei\\Co shu Nie - Zettai Zetsumei (sankansuki) [Zetsubou].osu");

	std::cout << "=== [find osu files] ===" << std::endl;
	
	ofp::findOsuFiles("C:\\Users\\Baio\\Desktop\\Games\\osu!\\Songs");
	return 0;
}
