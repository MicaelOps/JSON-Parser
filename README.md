# JSON-Parser
JSON Parser created for the minecraft-gameserver project

Since I don't instend to spread the project I woont show how to use it but the minecraft-gameserver project will. 

I will strive to make it as fast as my current negligible experience and knowledge allows.
I will certaintly rewrite this as many times i feel like until I am satisfied. There will probably be a new version every 3 months or less depending on my time and motivation.

Current rewrite count: 0 

Logs: (afterthoughts, rants, etc..)

25/11/2025 pt2
- Ok I finished the beta version of the JSON Parser.
- It is a bit ugly as it was a first time but I am sure I will rewrite soon because I will see drastic perfomance drops on my minecraft-server project and will not be able to cope with it.
- I will also read some json parsers in c++ to gain inspiration and to understand why they did the way they did.

25/11/2025
- I can't lie, parsing numbers in JSON is reminding me of that one Easy LeetCode problem. However in this case it is not just integers but also decimal numbers, floating points, etc... which make this 100x harder
- I may have to make a sacrifice in the precision of numbers.
- I feel like just slapping the json_numbers with a internal string called "original_data" to let programmers calculate their floating points with the precision they like. Of course, I would just implement a basic one...
- I think I will just implement a functionality for integers and will only move further if the minecraft project requires floating points to be used in JSON.
  
22/11/2025
- I am trying to not make a linear check of every character and instead use string.find() + string.find_first_of() + string.first_first_not_of() to navigate through the JSON but its not perfect.
- For JSON Strings like "{'name': 233}", it would work perfectly. Otherwise, json strings like  "////{///"name":  123123///}" would not shine. Maybe a few more tinkering and would be fine but alas I CBA
- I have been thinking of a new method for the next rewrite, If there are JSON_OBJECTS (aka map like values) wouldn't it be more efficient if i just attempt to find all ':' signatures and preemtively process those values and assign to their owners based of pos or working backwards?
- Additionally, instead of ':" why not ','?  
