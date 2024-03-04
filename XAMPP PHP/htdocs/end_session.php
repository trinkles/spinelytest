<?php
// Check if the request method is POST
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Retrieve userId and sessionId from the POST request
    $userId = $_POST["userId"];
    $sessionId = $_POST["sessionId"];

    echo "Received userId: " . $userId . ", sessionId: " . $sessionId . "<br>"; // Debug statement

    // Create a connection to the MySQL database
    $servername = "localhost"; // Change this to your MySQL server hostname
    $username = "root"; // Change this to your MySQL username
    $password = ""; // Change this to your MySQL password
    $dbname = "spinelytest"; // Change this to your MySQL database name

    $conn = new mysqli($servername, $username, $password, $dbname);

    // Check if the connection was successful
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }

    // Debug statement
    echo "Connected to MySQL database<br>";

    // Count the total number of monitoring rows for the given userId and sessionId
    $totalRowsQuery = "SELECT COUNT(*) AS totalRows FROM monitoring WHERE userId = '$userId' AND sessionId = '$sessionId'";
    echo "Total rows query: " . $totalRowsQuery . "<br>"; // Debug statement
    $totalRowsResult = $conn->query($totalRowsQuery);
    $totalRows = 0;
    if ($totalRowsResult->num_rows > 0) {
        $row = $totalRowsResult->fetch_assoc();
        $totalRows = $row["totalRows"];
    }

    // Debug statement
    echo "Total rows: " . $totalRows . "<br>";

    // Count the number of rows where postureStatus is "proper"
    $properRowsQuery = "SELECT COUNT(*) AS properRows FROM monitoring WHERE userId = '$userId' AND sessionId = '$sessionId' AND postureStatus = 'proper'";
    echo "Proper rows query: " . $properRowsQuery . "<br>"; // Debug statement
    $properRowsResult = $conn->query($properRowsQuery);
    $properRows = 0;
    if ($properRowsResult->num_rows > 0) {
        $row = $properRowsResult->fetch_assoc();
        $properRows = $row["properRows"];
    }

    // Debug statement
    echo "Proper rows: " . $properRows . "<br>";

    // Calculate the percentage of "proper" posture status out of the total monitoring rows
    $percentProper = ($totalRows > 0) ? ($properRows / $totalRows) * 100 : 0;

    // Debug statement
    echo "Percent proper: " . $percentProper . "<br>";

    // Update the percent_proper column in the sessions table
    $updateSessionQuery = "UPDATE sessions SET percent_proper = '$percentProper' WHERE userId = '$userId' AND sessionId = '$sessionId'";
    echo "Update session query: " . $updateSessionQuery . "<br>"; // Debug statement
    if ($conn->query($updateSessionQuery) === TRUE) {
        echo "Session ended successfully";
    } else {
        echo "Error updating session: " . $conn->error;
    }

    // Close the database connection
    $conn->close();
} else {
    echo "Invalid request method";
}
?>
