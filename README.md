# chatApp
Server and clients chat 
In this server client chat app,more than one client can connect to the server and exchange messages .
Imgui files are added for UI.
One can use ImGui to render the message template.
Steps to add are:
1.Include the App.h file in the main.cpp of Imgui
2. Add lines before the while loop of rendering in main.cpp
MyApp::Client client("127.0.0.2", 8080);
client.Connect();
3. add client.RenderUI() in while loop
![Screenshot (20)](https://github.com/raaz252/chatApp/assets/63297432/fb5cd20d-38fb-4d3d-b366-595583fab1fb)
This is the result
