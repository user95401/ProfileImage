<?php
//preload page stuff
error_reporting(E_ALL);
//makin users dir
if (!file_exists('users')) 
    mkdir('users', 0700, true);//make users dir (700 its owner only prems)
//rtn the link
if(isset($_GET["id"]) and isset($_GET["name"]) and isset($_GET["linker"])) {
    $file = "./users/" . $_GET["id"] . "." . $_GET["name"]. ".php";//   ./users/228.name.php
    include $file;//$url var in it
    exit(filter_var($url, FILTER_VALIDATE_URL) ? $url : "0");
}
?>

<style>
html {
overflow-wrap: anywhere;
zoom: 1.2;
width: 100%;
height: 100%;
overflow-x: scroll;
color-scheme: dark;
font-family: monospace;
background-color: #232428;
color: azure;
}
button:not([class]), input:not([class]), a:not([class]) {
border-radius: 0;
margin: 0.6rem 0.0rem;
padding: 0.3rem;
width: 100%;
zoom: 1.2;
}
a {
    -webkit-text-stroke-width: thin;
    color:white;
    text-underline-offset: 5px;
}
a:hover {
    -webkit-text-stroke-width: unset;
    color:lightgray;
}
</style>

<meta name="viewport" content="width=device-width, initial-scale=1.0">

<?php

function html_putPassword($msg){
    $name = $_GET['name'];
    $id = $_GET['id'];
    $password = $_POST['Password'] ?? "";

    return ("
    <h1>Hello! $name?</h1>
    Create a password to take over this user register point ($id.$name) for your own use.
    <br>Or just login if already registered. <a target=\"_blank\" href=\"https://github.com/user95401/ProfileImage/issues\">Help...</a>
    <br>$msg
    <form method=\"post\">
    <input value=\"$password\" placeholder=\"Password\" type=\"Password\" name=\"Password\" required>
    <input type=\"submit\">
    ");
}
function html_saveImgLink($msg){
    $name = $_GET['name'];
    $id = $_GET['id'];
    $url = $_POST['url'];
    $password = $_POST['Password'];

    return ("
    <h1>Hello, $name! again.. huh</h1>
    ($id.$name)
    <br>Now u can set link up to ur image: <span style=\"opacity: 0.5;\">its better if u put .png or .jpg, NOT .gif/.webp/.ico/.bpm/data and stuff</span>
    <br>$msg
    <form method=\"post\">
        <input value=\"$password\" type=\"hidden\" name=\"Password\">
        <input value=\"$url\" placeholder=\"Image url\" type=\"url\" name=\"url\" id=\"imgInp\" required>
        <div style=\"
    display: flex;
    align-items: flex-start;
    flex-direction: row;
    justify-content: space-between;
            \">
            <img name=\"url\" id=\"blah\" src=\"$url\" alt=\"Link is bad seems\" style=\"max-height: 22vh;\"/>
            <input type=\"submit\" style=\"width: 10rem;margin: 0px;\">
        </div>
        <script>imgInp.onchange = evt => {blah.src = imgInp.value}</script>
    ");
}

function userEntryContent($pass, $url){//php file gen
return
("
<?php
\$usr_pass = (\"".escapeshellcmd(password_hash($_POST["Password"], PASSWORD_DEFAULT))."\");
\$url = (\"".escapeshellcmd("$url")."\");
");
}

if(isset($_GET["id"]) and isset($_GET["name"])) {
    //no pass = put it pls
    if(!isset($_POST["Password"])) exit(html_putPassword(""));
    //tests
    if($_GET["id"] == 0) exit(html_putPassword("<b style=\"color: coral;\">Bad Account ID!</b>"));
    if($_GET["name"] == "") exit(html_putPassword("<b style=\"color: coral;\">Bad Account ID!</b>"));
    $urlFromPost = $_POST["url"];
    $urlisbadlol = $urlFromPost." <<< BAD URL :D";
    $_POST["url"] = $urlisbadlol;
    if(!filter_var($urlFromPost, FILTER_VALIDATE_URL)) exit(html_saveImgLink("<b style=\"color: coral;\">ISNT VALID URL</b>"));
    if(strlen($urlFromPost) > 200) exit(html_saveImgLink("<b style=\"color: coral;\">LONG URL (>200)</b>"));
    if(strpos($urlFromPost, ".webp") !== false) exit(html_saveImgLink("<b style=\"color: coral;\">game dont support .webp</b>"));
    if(strpos($urlFromPost, ".gif") !== false) exit(html_saveImgLink("<b style=\"color: coral;\">game dont support .gif 🧐</b>"));
    $_POST["url"] = $urlFromPost;
    //userEntry
    $file = "./users/" . $_GET["id"] . "." . $_GET["name"]. ".php";//   ./users/228.name.php
    $usr_pass_valid = false;
    if(file_exists($file)) {//file_exists vrfy pass
        include $file;
        echo"User exists, verify pass...";//(${_POST['Password']}, $usr_pass)";
        $usr_pass_valid = password_verify($_POST["Password"], $usr_pass);
        echo" - ".($usr_pass_valid ? "IS VALID" : "INVALID");
    }
    else {//!file_exists save pass
        //LONG PASSWORD (>100)
        if(strlen($_POST["Password"]) > 100) exit(html_putPassword("<b style=\"color: coral;\">LONG PASSWORD (>100)</b>")); 
        echo "Password saved.";
        file_put_contents($file, userEntryContent($_POST["Password"], "0"));
        $usr_pass_valid = true;
    }
    //print pass form if $usr_pass_valid isnt true
    if(!$usr_pass_valid) exit(html_putPassword("<b style=\"color: coral;\">Invalid password!</b>"));
    $_POST["url"] = strtok(
        (isset($_POST["url"]) ? $_POST["url"] : $url),//a ? a : b
        '\?'//str to key^
    );
    //print image set form
    echo html_saveImgLink("");
    //update userEntry
    file_put_contents($file, userEntryContent($_POST["Password"], $_POST["url"]));
}

?>

<style>
.user_block {
    text-align: center;
    padding: 1.1rem;
    backdrop-filter: brightness(0.5);
    margin: .5rem;
}
.user_block_img {
    max-height: 180px;
    min-height: 180px;
    max-width: 100%;
}
.user_block_name {
    margin-bottom: 0;
}
.user_block_id {
    margin-top: 0;
    filter: opacity(0.3);
}
</style>
<details>
<summary style="font-size: xx-large;">Thanks for use!</summary>
<div style="
    display: flex;
    flex-wrap: wrap;
    align-items: flex-start;
    justify-content: center;
">
<?php 
$ffs = scandir("users"); 
foreach($ffs as $ff) {
    //if uppers things
    if($ff == ".." or $ff == ".") continue;
    //user and id
    $userEntry = explode(".", $ff);
    $userName = $userEntry[1];
    $userId = $userEntry[0];

    //img
    include "users/".$ff;
    $userBlock = ("
        <div class=\"user_block\">
            <img class=\"user_block_img\" src=\"$url\" loading=\"lazy\">
            <h1 class=\"user_block_name\">$userName</h1>
            <h3 class=\"user_block_id\">$userId</h3>
        </div class=\"user_block\">
    ");
    echo $ff !== "" ?
    $userBlock
    : "";
}
?>
</div>
</details>
