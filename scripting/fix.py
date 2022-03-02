class fix:
    def json( str ):
        fix = [ 
            ( "0,\n        ", "0, " ),
            ( "1,\n        ", "1, " ),
            ( "[\n        0", "[ 0" ),
            ( "0\n      ]", "0 ]" ),
            #( "[\n    {", "[ {" ),
            #( "}\n  ]", "} ]" ),
            #( "    } ]", "  } ]" ),
            #( '      "map"', '    "map"' ),
            #( '      [', '    [' ),
            #( '      ]', '    ]' )
        ]

        for tuple in fix:
            str = str.replace( tuple[ 0 ], tuple[ 1 ] )
        return str