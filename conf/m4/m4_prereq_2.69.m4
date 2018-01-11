

m4_version_prereq([2.69], [] ,[

m4_ifndef([AS_VAR_COPY],
  [m4_define([AS_VAR_COPY],[AS_LITERAL_IF([$1[]$2], [$1=$$2], [eval $1=\$$2])])])

m4_ifndef([AS_VAR_APPEND],
  [m4_define([AS_VAR_APPEND],[$1=$$1$2])])

# m4_escape(STRING)
# -----------------
# Output quoted STRING, but with embedded #, $, [ and ] turned into
# quadrigraphs.
#
# It is faster to check if STRING is already good using m4_translit
# than to blindly perform four m4_bpatsubst.
#
# Because the translit is stripping quotes, it must also neutralize
# anything that might be in a macro name, as well as comments, commas,
# and parentheses.  All the problem characters are unified so that a
# single m4_index can scan the result.
#
# Rather than expand m4_defn every time m4_escape is expanded, we
# inline its expansion up front.
m4_define([m4_escape],
[m4_if(m4_index(m4_translit([$1],
   [[]#,()]]m4_dquote(m4_defn([m4_cr_symbols2]))[, [$$$]), [$]),
  [-1], [m4_echo], [_$0])([$1])])

m4_define([_m4_escape],
[m4_changequote([-=<{(],[)}>=-])]dnl
[m4_bpatsubst(m4_bpatsubst(m4_bpatsubst(m4_bpatsubst(
          -=<{(-=<{(-=<{(-=<{(-=<{($1)}>=-)}>=-)}>=-)}>=-)}>=-,
        -=<{(#)}>=-, -=<{(@%:@)}>=-),
      -=<{(\[)}>=-, -=<{(@<:@)}>=-),
    -=<{(\])}>=-, -=<{(@:>@)}>=-),
  -=<{(\$)}>=-, -=<{(@S|@)}>=-)m4_changequote([,])])


# m4_map_args_w(STRING, [PRE], [POST], [SEP])
# -------------------------------------------
# Perform the expansion of PRE[word]POST[] for each word in STRING
# separated by whitespace.  More efficient than:
#   m4_foreach_w([var], [STRING], [PRE[]m4_defn([var])POST])
# Additionally, expand SEP between words.
#
# As long as we have to use m4_bpatsubst to split the string, we might
# as well make it also apply PRE and POST; this avoids iteration
# altogether.  But we must be careful of any \ in PRE or POST.
# _m4_strip returns a quoted string, but that's okay, since it also
# supplies an empty leading and trailing argument due to our
# intentional whitespace around STRING.  We use m4_substr to strip the
# empty elements and remove the extra layer of quoting.
m4_define([m4_map_args_w],
[_$0(_m4_split([ ]m4_flatten([$1])[ ], [[        ]+],
               m4_if(m4_index([$2$3$4], [\]), [-1], [[$3[]$4[]$2]],
                     [m4_bpatsubst([[$3[]$4[]$2]], [\\], [\\\\])])),
     m4_len([[]$3[]$4]), m4_len([$4[]$2[]]))])

m4_define([_m4_map_args_w],
[m4_substr([$1], [$2], m4_eval(m4_len([$1]) - [$2] - [$3]))])



# m4_ifblank(COND, [IF-BLANK], [IF-TEXT])
# m4_ifnblank(COND, [IF-TEXT], [IF-BLANK])
# ----------------------------------------
# If COND is empty, or consists only of blanks (space, tab, newline),
# then expand IF-BLANK, otherwise expand IF-TEXT.  This differs from
# m4_ifval only if COND has just whitespace, but it helps optimize in
# spite of users who mistakenly leave trailing space after what they
# thought was an empty argument:
#   macro(
#         []
#        )
#
# Writing one macro in terms of the other causes extra overhead, so
# we inline both definitions.
m4_define([m4_ifblank],
[m4_if(m4_translit([[$1]],  [ ][        ][
]), [], [$2], [$3])])

m4_define([m4_ifnblank],
[m4_if(m4_translit([[$1]],  [ ][        ][
]), [], [$3], [$2])])


])

