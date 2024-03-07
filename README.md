# spinelytest
### Disclaimer:
### This is done in Windows. Commit changes on how this is done in Mac or Linux.
### ESP32 does not connect with 5G networks, only 2.4GHz

## Description
Spinely (test) consists of  **2 different setups --hence 2 different folders**. 
1. **From ESP32**: An interface is accessed via hotspot's IP (`172.20.10.4`). Any end device connecting to the hotspot can access this. Arduino (`.ino`) file can be placed anywhere in directory.
2. **From XAMPP**: A localhost server is run once Apache and SQL are started. Its `htdocs folder` consists of PHP files only (not `.ino` file).

## ESP32 Testing Setup
- Connect to the 2.4 GHz hotspot (SSID: `AA`)
- Open your browser and go to this URL: `172.20.10.4`
- Make sure your XAMPP testing setup is working (refer to **XAMPP Testing Setup**)
- To edit the Arduino sketch, refer to **ESP32 Development Setup**

## ESP32 Development Setup
- Save the folder named `localspinelyAsync` anywhere
- Modify the `.ino` file using Arduino IDE
- Separately, commit changes to `localspinelyAsync` in github

## XAMPP Testing Setup
- Open XAMPP
- Configure `Apache (httpd.conf)` via Notepad or any text editor. Change Apache's port from `Listen 80` to `Listen 8000`.
- Start Apache and SQL
- To access PHPmyAdmin, go to `http://localhost:8000/phpmyadmin`
- Create new database named `spinelytest`. After creating, import the `.sql` file.
- If there are new sql entries in your version of `spinelytest`, just export with name "spinelytest_<username>.sql" `add`, `commit` with message, and `push` to this repository.
