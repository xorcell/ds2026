#page()[
  #set text(font:"Poppins")
  #place(horizon+left, dx:-15pt)[#line(length: 90%, angle:90deg)]
  #align(right)[
    PETRUCCI Titouan - 2210009
  ]
  #place(horizon, dy:-50pt, dx: 10pt)[
    #text(size:3em, font: "Poppins", weight: "bold")[
      Creating a \
      Basic File Transfer Protocol 
    ]

    #text(font: "Adwaita Mono", size:1.25em)[
      using the C programming language
    ]
  ]

  #align(bottom+right)[
    University of Sciences and Technology of Hanoi
    ]
]


#set page(numbering: "1")
#set heading(numbering:"I.1.a/")
#set text(font:"DejaVu Serif")

#page()[#outline()]

= Protocol
To create the protocol, we need to establish what are the real needs
== Requirements
- Clients would be able to download files from the server
- Clients can download multiple files in a row
- Protocol need to support upgrade (if we want to upgrade it)
== Diagram

#import "@preview/chronos:0.2.1"

#chronos.diagram({
  import chronos : *
  _par("Client")
  _par("Server")

  _seq("Client", "Server", comment:"BFT 1.0 FILE /filename.txt", enable-dst:true)
  _seq("Server", "Server", comment:"Checking if file exists")

  _alt("File exists",
  {
    _seq("Server", "Client", dashed:true, comment: "BFT 1.0 FILE /filename.txt 200 <raw_data>")
  },
  "File doesn't exist",
  {
    _ret(comment:"BFT 1.0 FILE /filename.txt 404 No such file")
  })
  
  
})

Error code 200 for success and 404 for non-existant data are taken from the HTTP protocol, because it works well and everyone knows them.

= Basic Implementation

== Server
#block(fill:luma(245), radius: 3pt, inset:4pt)[
  #raw(read("server.c"), lang:"c", block:true)
]
== Client
#block(fill:luma(245), radius: 3pt, inset:4pt)[
  #raw(read("client.c"), lang:"c", block: true)
]

#import "@preview/cmarker:0.1.7": render

#let tuto = read("TUTO.md")

#render(tuto)


