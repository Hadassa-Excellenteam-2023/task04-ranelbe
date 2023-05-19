# task04-ranelbe 

<h2>City Finder</h2>

<p>
This program calculates all the cities within a given radius of a specified city, based on their coordinates. It utilizes efficient data structures to perform the calculations and provides a sorted list of cities based on their distance from the specified city. 
Additionally, it provides the total number of cities within the radius range and the number of cities located north of the selected city.
</p>

<h2>Input File</h2>

<p>
  The program requires an input file, <code>txt.data</code>, which contains a list of cities and their corresponding coordinates. 
  The coordinates are represented as (x,y) values. 
</p>

<h2>Data Structures</h2>

<p>The program uses the following data structures for efficient searching and sorting:</p>

<ul>
  <li><code>unordered_map&lt;string, Location&gt;</code>: This unordered map allows for fast city lookup using the city name as the key. The <code>Location</code> object stores the coordinates of the city.</li>
  <li><code>multimap&lt;Location, string, LessX&gt;</code>: This multimap maps locations to city names and is sorted in ascending order based on the x-coordinate values.</li>
  <li><code>multimap&lt;Location, string, LessY&gt;</code>: Similar to the previous multimap, this one is sorted in ascending order based on the y-coordinate values.</li>
  Using these structures makes it easier for us to find the intersection square
</ul>
