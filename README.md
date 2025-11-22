# JSON-Parser
JSON Parser created for the minecraft-gameserver project

Since I don't instend to spread the project I woont show how to use it but the minecraft-gameserver project will. 

I will strive to make it as fast as my current negligible experience and knowledge allows.
I will certaintly rewrite this as many times i feel like until I am satisfied. There will probably be a new version every 3 months or less depending on my time and motivation.

Current rewrite count: 0 

Logs: (afterthoughts, rants, etc..)

22/11/2025
- I am trying to not make a linear check of every character and instead use string.find() + string.find_first_of() + string.first_first_not_of() to navigate through the JSON but its not perfect.
- For JSON Strings like "{'name': 233}", it would work perfectly. Otherwise, json strings like  "////{///"name":  123123///}" would not shine. Maybe a few more tinkering and would be fine but alas I CBA
- I have been thinking of a new method for the next rewrite, If there are JSON_OBJECTS (aka map like values) wouldn't it be more efficient if i just attempt to find all ':' signatures and preemtively process those values and assign to their owners based of pos or working backwards?
- Additionally, instead of ':" why not ','?  
