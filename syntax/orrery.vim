" Vim syntax file
" Language:	orrery
" Maintainer:	Sakamoto Yuta <sksat.tec@gmail.com>
" Last Change:	2018
" Remark:

" 参考: http://vim-jp.org/vimdoc-ja/usr_44.html
syn case match " 大文字・小文字を区別する

syn keyword orreryKeyword import return true
syn keyword orreryTodoComment TODO contained
syn region orreryLineComment start=+//+ end=+$+ contains=orreryTodoComment
syn region orreryMultiComment start=+/\*+ end=+\*/+
syn region orreryString start=/"/ end=/"/
syn match orreryDecNumber "\<[0-9]*"

hi link orreryKeyword		Keyword
hi link orreryTodoComment	Todo
hi link orreryLineComment	Comment
hi link orreryMultiComment	Comment
hi link orreryString		String
hi link orreryDecNumber		Number
