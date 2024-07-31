<?php
// Set the default timezone
date_default_timezone_set('Europe/Madrid');

// Get the current date
$currentDate = date('Y-m-d H:i:s');

// HTML content with the current date
$htmlContent = "
<!DOCTYPE html>
<html lang=\"en\">
<head>
    <meta charset=\"UTF-8\">
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
    <title>Current Date</title>
</head>
<body>
    <h1>The current date and time is: $currentDate</h1>
</body>
</html>
";

// Print the HTML content
echo $htmlContent;
?>
