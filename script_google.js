function doGet(e) { 
  Logger.log( JSON.stringify(e) );  // view parameters

  var result = 'Ok'; // assume success

  if (e.parameter == undefined) {
    result = 'No Parameters';
  }
  else {
    var id = '15Ypm4fBjZnEcq3Ut1kzX6g2Jk1dydYHYbBieD3Ll6GU'; // Spreadsheet ID
    var sheet = SpreadsheetApp.openById(id).getActiveSheet();
    var newRow = sheet.getLastRow() + 1;
    var rowData = [];
    var now= new Date();
    rowData[0] = Utilities.formatDate(now, 'Etc/GMT', 'dd/MM/yyyy');
    for (var param in e.parameter) {
      Logger.log('In for loop, param='+param);
      var value = stripQuotes(e.parameter[param]);
      //Logger.log(param + ':' + e.parameter[param]);
      switch (param) {
        case 'name': //Parameter
          rowData[1] = value; //Value in column B
          break;
        case 'in': //Parameter
          rowData[2] = value; //Value in column B
          break;
        case 'out': //Parameter
          rowData[3] = value; //Value in column B
          break;
        default:
          result = "unsupported parameter"+param;
      }
    }
    Logger.log(JSON.stringify(rowData));

    // Write new row below
    var newRange = sheet.getRange(newRow, 1, 1, rowData.length);
    newRange.setValues([rowData]);
  }

  // Return result of operation
  return ContentService.createTextOutput(result);
}

/**
* Remove leading and trailing single or double quotes
*/
function stripQuotes( value ) {
  return value.replace(/^["']|['"]$/g, "");
}