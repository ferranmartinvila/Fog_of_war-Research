<h1> Fog of War Research </h1>

<h4>Visits</h4>
<script type="text/javascript" src="https://counter3.fcs.ovh/private/counter.js?c=ulymtan43z818x2gxpkb2m4jx6rlhtxx"></script>
<noscript><a href="https://www.freecounterstat.com" title="website counters"><img src="https://counter3.fcs.ovh/private/freecounterstat.php?c=ulymtan43z818x2gxpkb2m4jx6rlhtxx" border="0" title="website counters" alt="website counters"></a></noscript>

<h2> Index </h2>
<ul>
<li> <a href="#INTRO"> Introduction </a> </li>
<li> <a href="#CONCEPT"> Concept </a> </li>

<li> <a href="#TYPES"> Fog Types </a> </li>
<li> <a href="#ORIGINS"> Origins </a> </li>
<li> <a href="#TUTORIAL"> Tutorial </a> </li>
<li> <a href="#REFERENCES"> References </a> </li>
<ol/>
</ul>

<h1 id="INTRO"> Introduction </h1>
<ul>
<p>Fog of war is a common concept found in many combat-based real-time and turn-based strategy games. It simulates the unknowns of the battle and provides an exploration incentive to uncover them, forcing the player to rely on imperfect and incomplete information of the battlefield and sometimes improvise based on intuition and common sense.</p>

<p>Here we are going to talk about the concept of the Fog of War and all its variables, the origins, how board games and videogames adapt the concept to engage the user, and finally a tutorial of how to do a fog of war for an RTS game.</p>
</ul>

<h1 id="CONCEPT"> Fog of War Concept </h1>
<ul>
<p>The fog of war is a term used to describe the uncertainty in the situation experienced by participants in military operations. It isn’t limited at the own capability, so it determinates the adversary capability too.</p>

<p>So fog of war is directly related with the fog in a real battle camp and the ambiguity that it generates, but when this concept is transferred to the world of games is not necessary simulated with fog. </p>

<p>For example, in Battleship the famous board game, there’s no fog but the concept of war ambiguity is present, in fact, is the principal feature of the game.</p>


<img src="Research_docs/battleship.jpg"/>

<p>In videogames is practically the same. It’s true that a lot of videogames simulate the fog of war concept directly with fog but some videogames use other methods. For example, BRAIN / OUT is a shooter that simulates the war ambiguity hiding the players that aren’t doing noisy actions.</p>
<p>
<img src="Research_docs/brain_nope.png"/>
<img src="Research_docs/brain_yes.png"/>

<p>Now that we know what really means Fog of War let’s talk about the different ways in which the ambiguity can affect the user and change the game flow.</p>
</ul>

<h1 id="TYPES"> Fog Types </h1>
<ul>

<p>The ambiguity that the fog of war generates can vary according to the level at which participants are engaged.</p>
<p>The different ambiguity levels are:</p>
<ul>
<li><h4> Grand Strategic </h4></li>
<ul>
<p>Ambiguity is related to the political intent, capabilities and logistical strengths of an adversary. Basically we know that there’s an adversary but we don’t know nothing about it. </p>
<p>A game with this type of fog of war is Age of Empires II. Where we know all about your civilization but nothing about the enemy.</p>
<img src="Research_docs/aoe2_gm.jpg"/>
</ul>
<li><h4> Military Strategic </h4></li>

<ul>
<p>Ambiguity is related to the structure, strength, capability, and disposition of own and adversary offensive and defensive assets. The own assets ambiguity can be generated by a bad report of it or unwillingness to watch for them. </p>
<p>A good example of this ambiguity type is Call of Duty MW3 in hardcore mode. In normal mode the user knows the location of the allied players and the avatar stats, as you can see in the first screenshot. But in hardcore mode the minimap and all the UI related with the stats is hidden as you can see in the second screenshot. So the second frame simulates the military strategic ambiguity.</p>
<img src="Research_docs/cod_easyore.png"/>
<img src="Research_docs/cod_hardcore.png"/>
</ul>
<li><h4> Operational </h4></li>
<ul>
<p>Ambiguity is related with the adversary like the grand strategic but is coupled with own directive ambiguity. So the commander doesn’t have the full sight of the strategic imperative</p>
<p>The example game is Dragons of Atlantis. In this game you can send your units to attack the enemy, but you don’t know the result of the operation since the units return to your domains or a messenger is send.</p>
<p>The first screenshot shows the order and the second shows the result.</p>
<img src="Research_docs/atl_drag_prep.png"/>
<img src="Research_docs/atl_drag_result.png"/>
</ul>
<li><h4> Tactical </h4></li>
<ul>
<p>In this case the ambiguity is around several factors at the tactical level. The ambiguity can be related with the interaction between allied troops, their status and their intentions. This lack of comprehension of the tactical environment by the commander aren’t necessary generated by factors inherent to battle. </p>
<p>In this case the late game of Spore is a good example of tactical ambiguity. In spore you can leave your planet to explore others, but other creatures can attack your planet which you don’t have contact.</p>
<p>Screenshot of the player leaving his planet.</p>
<img src="Research_docs/spore_travel.png"/>
</ul>
</ul>
<h1 id="ORIGINS"> Origins </h1>
<ul>
<p>Okay, we have seen all the different features of the fog of war and which mechanics use the games to adapt the concept. But who was the first person that thinks that use fog of war in a game should be a good idea?</p>
<p></p>
<p>The first board game that uses fog of war was Kriegsspiel, that means war game in German. This game was created by Lieutenant von Reisswitz in 1824 for training officers of the Prussian army. The invent that Lieutenant craft wasn’t directed only for playing but is considered the grandfather of modern war-games because he established several conventions for war gaming.</p>
<img src="Research_docs/kniegsel_tab.png"/>
<p>Entering to the digital era, we have Tanktics. The first digital war-game created in 1977 by Chris Crawford’s and published for Commodore PET in 1978. The game simulates a two-player tank battle on a hex grid. This was the first videogame that uses fog of war and for that was criticized. The fog of war in Tanktics generates a Military Strategic ambiguity because the game doesn’t show the enemy coordinates since your tanks don’t see them.</p>
<p>In the first screenshot of  Tanktics we can see three ally Tanks (A,B,C) that the player controls and the enemy tanks (A,B,C,D,E,F) that are in unknown coordinates. In the second frame the player has discovered A, C and F tanks. To know more about Tanktics location system and how to play it, go <a href="http://www.atarimania.com/game-atari-tanktics_s5270.html">here</a> to see the manual.</p>
<img src="Research_docs/tanktics_1.png"/>
<img src="Research_docs/tanktics_2.png"/>
</ul>
<h1 id="TUTORIAL"> Tutorial </h1>
<ul>
<p>Now we are experts of fog of war so let’s craft our fog of war for a RTS videogame like Age of Empires or StarCraft.</p>
<p>This tutorial of how to make a basic fog of war for a RTS videogame is focused to people that knows the basic concepts of c/c++ programming. </p>
<p>First we download the handout from <a href="https://github.com/ferranmartinvila/Fog_of_war-Research/releases/tag/v.0.9">here</a>.</p>
<p>To check if you do the TODOs correctly check <a href="https://github.com/ferranmartinvila/Fog_of_war-Research/releases/tag/v.1.0">this</a> release with the tutorial solved. </p>
<p>If you don't get the correct result and don't know how to solve it check the Solutions folder inside the Handout. There you can find screenshoots of the TODOs code solved.</p>
<p>Ok we have the material, let’s work with it!</p>


<h2 id="TODO_1"> TODO 1: Create the fog layer! </h2>
<ul>
<p>Allocate an array of FOG_TYPE for the fog_layer in the fog of war module.</p>
<p>Array size is the same as a map layer, so get map width and map height from App->map.data .</p>
<p>Iterate the allocated array and fill it with DARK_FOG type.</p>
<p>When all the past objectives are cleared uncomment the code of ClearFogLayer method in fog of war module.</p>
<p>This first TODO don't have a visual output so let's do the second to check it.</p>
</ul>
<h2 id="TODO_2"> TODO 2: Map optimization! </h2>
<ul>
<p>Improve the GetFogID method in fog of war module, to get the FOG_TYPE of the map tiles.</p>
<p>When the GetFogID method is working, call it in the map draw method to check the tiles FOG_TYPE.</p>
<p>The tiles_in_view vector are in map coordinates so you can pass them to the GetFogID method without any transformation.</p>
<p>If the FOG_TYPE is DARK_FOG the tile is covered of opaque fog so its not necessary to draw it.</p>
<h3>Before</h3>
<img src="Research_docs/Todo_2_gif.gif"/>
<h3>After</h3>
<img src="Research_docs/Todo%203/entity_no.gif"/>
</ul>
<h2  id="TODO_3"> TODO 3: Entities optimization!</h2>
<ul>
<p>Improve the entity Draw method to draw the entity only when is necessary.</p>
<p>Use the fog_type variable to get the FOG_TYPE in the entity coordinates.</p>
<p>Check the entity.type variable to know the entity diplomacy and respect the next rules:</p>
<ul>
<li><p>ALLY entities are always drawn.</p></li>
<li><p>NEUTRAL entities are not drawn when FOG_TYPE is DARK_FOG.</p></li>
<li><p>ENEMY entities are only drawn when there’s NO_FOG.</p></li>
</ul>
<h3>Before</h3>
<img src="Research_docs/Todo%203/entity_no.gif"/>
<h3>After</h3>
<img src="Research_docs/Todo%203/entity_yes.gif"/>
</ul>

<h2 id="TODO_4"> TODO 4: Update alpha layer! </h2>
<ul>
<p>Alpha layer is built in the GenerateFogOfWar method, and is a huge array of AlphaCells that look like this:</p>
<img src="Research_docs/code_img.png"/>
<p>Collect the candidates of the fog_quadtree to get the alpha cells in camera.</p>
<p>CollectCandidates quadtree method fills the vector you pass with the alpha cells inside the specified area.</p>
<p>Iterate the filled vector and call FogBlit method from render module.</p>
<p>The cell size is the size of the AlphaCell walls in pixels, calculated when the fog alpha layer is generated. The variable name is alpha_cell_size.</p>
<h3>Before</h3>
<img src="Research_docs/todo_4_no_draw.png"/>
<h3>After</h3>
<img src="Research_docs/todo_5_dark.png"/>
</ul>

<h2 id="TODO_5"> TODO 5: Make ally entities clear alpha layer! </h2>
<ul>
<p>In entity draw method check if the entity is an ally.</p>
<p>If is an ally, clear alpha layer around vision area with 0 alpha value. </p>
<p>To clear alpha layer use ClearAlphaLayer method from fog of war module.</p>
<h3>Before</h3>
<img src="Research_docs/todo_5_dark.png"/>
<h3>After</h3>
<img src="Research_docs/todo_5_light.png"/>
</ul>
<h2 id="TODO_6"> TODO 6: Make player check his vision area!</h2>
<ul>
<p>Fill CheckVisionArea entity method and call it from player update.</p>
<p>Use the public entities quadtree from the entities manager to collect the candidates inside the vision area.</p>
<p>CollectCandidates quadtree method fills the vector you pass with the entities inside the specified area.</p>
<p>Check the CollectCandidates method used in the TODO 4 to get a reference.</p>
<p>This TODO don't have a visual output but you can use LOG to print something when an entity enter the player area.</p>
<img src="Research_docs/todo_6_in.png"/>
</ul>

<h2 id="TODO_7"> TODO 7: Clear fog area when a neutral entity is detected!</h2>
<ul>
<p>Iterate the entities collected in the last TODO and check if there’s a neutral entity.</p>
<p>If there’s one, clear alpha layer around its vision area with MID_ALPHA value. Is the same procees of TODO 5.</p>
<p>Then you need to clear the fog layer we created before to render all around the target render area. </p>
<p>So use ClearFogLayer method in fog of war module to clear target render area with GRAY_FOG.</p>
<h3>Before</h3>
<img src="Research_docs/Todo%207/no_pop_gif.gif"/>
<h3>After</h3>
<img src="Research_docs/Todo%207/pop_gif.gif"/>
</ul>

<h2 id="TODO_8"> TODO 8: Update alpha layer!</h2>
<ul>
<p>To update alpha layer, you just need to check if there’s any alpha cell in camera with a higher value than the MID_ALPHA value.</p>
<p>If theres one, equal its alpha value to MID_ALPHA.</p>
<h3>Before</h3>
<img src="Research_docs/alpha_no_update.png"/>
<h3>After</h3>
<img src="Research_docs/alpha_update.png"/>
</ul>

<h2>Extra Work!</h2>
<ul>
<p>Now that you have a functional RTS fog of war system let’s upgrade it!</p>
<p>Update the ClearAlphaLayer method from fog of war module to have a smoothest result.</p>
<p>Check the solution release and active the glow mode( F2 ) to see one possible solution.</p>
<h3>Before</h3>
<img src="Research_docs/fog_not_smooth.png"/>
<h3>After</h3>
<img src="Research_docs/fog_smooth.png"/>
</ul>
</ul>

<h1 id="REFERENCES"> References </h1>
<ul>
<li><p><a href="http://dictionary.sensagent.com/Fog%20of%20war/en-en">Link</a> about fog of war concept</p></li>
<li><p><a href="https://en.wikipedia.org/wiki/Wargaming#Early_years_to_Kriegsspiel">Link</a> about Kriegsspiel (first war board game)</p></li>
<li><p><a href="https://archive.org/details/Tanktics_1981_Avalon_Hill">Link</a> about Tanktics (first war videogame)</p></li>
<li><p><a href="http://stackoverflow.com/questions/13654753/sdl-drawing-negative-circles-fog-of-war">Link</a> to another interesting fog of war tutorial</p></li>
</ul>
