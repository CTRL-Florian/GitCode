<h1>GitCode: A Git Copy</h1>

<br>
<div style="text-align: center;">
  <img src="gitCode.png" alt="Logo" />
</div>

<br>
<h2>Graph</h2>
<embed src="GitCodeGraph.pdf" type="application/pdf" width="600" height="800">

<br>
<h2>Commands:</h2>
<pre>
- gitCode init
- gitCode cat-file (-t | -s | -p | -e) &lt;object_sha1_hash&gt;
- gitCode hash-object (-w | ) &lt;filename&gt;
- gitCode ls-tree (--name-only | ) &lt;tree_sha1_hash&gt;
- gitCode write-tree
</pre>

<h2>Requirements</h2>
<pre>
- zlib
- OpenSSL
</pre>

<h2>Install guide:</h2>
<pre>
1. Install vcpkg [vcpkg](https://github.com/microsoft/vcpkg). 
2. Install zlib (library) using vcpkg: 
    bash: vcpkg install zlib 
  
3. Install OpenSSL using vcpkg: 
    bash: vcpkg install openssl
5. Integrate vcpkg with Visual Studio: 
    bash: vcpkg integrate install
7. Open project / solution in Visual Studio .
8. Build solution.
9. Add builddirectory to you path.

--> You can now use the gitCode commands from everywhere.
</pre>