<?php
// Database connection and configuration
$servername = "localhost";
$username_db = "root"; // Changed to avoid conflict with form username
$password_db = "";
$dbname = "spinelytest";

// Create connection
$conn = new mysqli($servername, $username_db, $password_db, $dbname);

// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}

// Fetch user data based on the provided username
if ($_SERVER["REQUEST_METHOD"] == "POST") {
  $loginUsername = $_POST['loginUsername']; // Use loginUsername to avoid confusion with database username

  // Prepare and bind SQL statement
  $stmt = $conn->prepare("SELECT username FROM users WHERE username = ?");
  $stmt->bind_param("s", $loginUsername);

  // Execute SQL statement
  $stmt->execute();

  // Get result
  $result = $stmt->get_result();

  // Check if user exists
  if ($result->num_rows > 0) {
    // User found, return username
    $row = $result->fetch_assoc();
    $username = $row['username'];
    echo $username; // Return username directly
  } else {
    // User not found
    echo "User not found";
  }

  // Close prepared statement
  $stmt->close();
}

// Close database connection
$conn->close();
?>
