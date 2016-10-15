========================================================================
			WIN32 APPLICATION : Simple Money Lover
========================================================================

- Submitted by: Đoàn Hiếu Tâm
- Student ID: 1412477
- Email: nhoxbypass@gmail.com
- Time spent: 5 hours in total

========================================================================

My Money Lover is an Windows application that allows user to note every thing when they spend money to do something.

## User Stories
The following **required** functionality is completed:

* [x] User can successfully add new record (consist of amount of money that user has spent, spending type and description about this record) and show it to list view
* [x] User can click on item of Listview to see and Press DELETE key to delete an existing record.
* [x] User can clear all list view item also all record that stored in program.
* [x] Write list item to text file and load data back when user open program again, any changes when the program run will be reflected in the data
* [x] User can see the CHART that show the ratio of every spending type in the total amount of money that user has spent before.
The CHART will be colored in different colors, and the percentage of every type will be showed.


The following **optional** features are implemented:
* [x] User can Hover mouse on each part of the CHART to see which type it is
* [x] User can use Menu or HOTKEY like Ctrl+N to create new item and insert it to data, Ctrl+L to clear all record
* [x] Tweak the style improving the UI / UX, play with static text colors, icon or backgrounds

## Video Walkthrough: https://youtu.be/ClPITwBbpKU

## Main flow:
 + Goku run the app see the Chart and all records that he save before in Listview
 + Goku and click "THÊM" button (or Ctrl + N), the Edit control enabled allow him to type 100000 and "Breakfast" in Money and Description Editcontrol
 and choose "Ăn sáng" in the combobox, then, click "LƯU". Application will insert this record to list data and show on Listview
 + Vegeta run the app, click on an item in Listview, and press DELETE key to delete this record
 + Vegeta press Ctrl + C, and click OK in the dialog showed to confirm clear all record in Listview and List data. He also can do this action by click Clear on Menu
 + Black move the mouse to hover on part of the CHART to see which type it belong to.

## Additional flow:
 + Freeza enter "TEST", "a123", or "%#$^^" to Money EditControl, your character will not be recorded, and a warning message appear
 + Buu enter number like -3, -5, 0, 1.5, 3.1416 to Money EditControl, you will receive a error message "Amount of money must greater than zero!"
 + Beerus leave the Money, Description Edit control empty, you will receive a error message "Amount of money or Description must not be empty!"