<?php
date_default_timezone_set('Europe/Madrid');

$currentDate = date('Y-m-d H:i:s');

$htmlContent = "
<!DOCTYPE html>
<html lang=\"en\">
<head>
    <meta charset=\"UTF-8\">
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
    <title>Current Date</title>
</head>
<body>
    <h3>The current date and time is: $currentDate</h3>
</body>
</html>
";

echo $htmlContent;
?>
