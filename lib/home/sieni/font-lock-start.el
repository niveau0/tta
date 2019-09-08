(setq font-lock-maximum-decoration t)
(require 'font-lock)
(add-hook 'emacs-lisp-mode-hook 'turn-on-font-lock)
(add-hook 'mail-setup-hook 'turn-on-font-lock)
(add-hook 'dired-mode-hook 'turn-on-font-lock)
(add-hook 'emud-mode-hook 'turn-on-font-lock)
(add-hook 'tcl-mode-hook 'turn-on-font-lock)
(add-hook 'perl-mode-hook 'turn-on-font-lock)

(defvar emud-font-lock-keywords nil
  "Font lock keywords for emud mode.")

(setq font-lock-face-attributes
      '((font-lock-comment-face "firebrick")
	(font-lock-string-face "MediumTurquoise")
	(font-lock-keyword-face "MediumTurquoise")
	(font-lock-function-name-face "MediumTurquoise")
	(font-lock-variable-name-face "MediumTurquoise")
	(font-lock-type-face "khaki")
	(font-lock-reference-face "ForestGreen")))

(let ((c-keywords
       "break\\|continue\\|do\\|else\\|for\\|if\\|return\\|switch\\|while")
      (c-type-types
       (concat "auto\\|c\\(har\\|onst\\)\\|double\\|e\\(num\\|xtern\\)\\|"
	       "float\\|int\\|long\\|register\\|"
               "mapping\\|object\\|mixed\\|string\\|varargs\\|nomask\\|"
               "public\\|private\\|"
	       "s\\(hort\\|igned\\|t\\(atic\\|ruct\\)\\)\\|typedef\\|"
	       "un\\(ion\\|signed\\)\\|vo\\(id\\|latile\\)"))	; 6 ()s deep.
      (c++-keywords
       (concat 
        "asm\\|break\\|c\\(atch\\|ontinue\\)\\|d\\(elete\\|o\\)\\|"
        "else\\|for\\|if\\|new\\|operator\\|"
;        "p\\(r\\(ivate\\|otected\\)\\|ublic\\)\\|return\\|"
        "protected\\|return\\|"
        "s\\(izeof\\|witch\\)\\|t\\(h\\(is\\|row\\)\\|ry\\)\\|while"))
      (c++-type-types
       (concat 
        ;; lpc stuff
        "mapping\\|object\\|mixed\\|string\\|varargs\\|nomask\\|"
        "public\\|private\\|"
        "auto\\|bool\\|c\\(har\\|lass\\|o\\(mplex\\|nst\\)\\)\\|"
        "double\\|e\\(num\\|xtern\\)\\|f\\(loat\\|riend\\)\\|"
        "in\\(line\\|t\\)\\|long\\|register\\|"
        "s\\(hort\\|igned\\|t\\(atic\\|ruct\\)\\)\\|"
        "t\\(emplate\\|ypedef\\)\\|un\\(ion\\|signed\\)\\|"
        "v\\(irtual\\|o\\(id\\|latile\\)\\)"))		; 11 ()s deep.
      (ctoken "[a-zA-Z0-9_:~]+"))
 (setq c-font-lock-keywords-1
  (list
   ;;
   ;; Fontify filenames in #include <...> preprocessor directives.
   '("^#[ \t]*include[ \t]+\\(<[^>\"\n]+>\\)" 1 font-lock-string-face)
   ;;
   ;; Fontify function macro names.
   '("^#[ \t]*define[ \t]+\\(\\(\\sw+\\)(\\)" 2 font-lock-function-name-face)
   ;;
   ;; Fontify otherwise as symbol names, and the preprocessor directive names.
   '("^\\(#[ \t]*[a-z]+\\)\\>[ \t]*\\(\\sw+\\)?"
     (1 font-lock-reference-face) (2 font-lock-variable-name-face nil t))
   ;; lpc inherit
   '("^inherit" . font-lock-reference-face)
   '("^inherit[ \t]+\\(<[^>\"\n]+>\\)" 1 font-lock-string-face)


   ;;
   ;; Fontify function name definitions (without type on line).
   (list (concat "^\\(" ctoken "\\)[ \t]*(") 1 'font-lock-function-name-face)
   ))

 (setq c-font-lock-keywords-2
  (append c-font-lock-keywords-1
   (list
    ;;
    ;; Fontify all storage classes and type specifiers (before declarations).
    (cons (concat "\\<\\(" c-type-types "\\)\\>") 'font-lock-type-face)
    ;;
    ;; Fontify variable/structure name declarations and definitions, or
    ;; function name declarations (plus definitions with type on same line).
    (list (concat "\\<\\(" c-type-types "\\)[ \t*]+"
		  "\\(" ctoken "[ \t*]+\\)*"
		  "\\(" ctoken "\\)[ \t]*\\((\\)?")
	  9
	  '(if (match-beginning 10)
	       font-lock-function-name-face
	     font-lock-variable-name-face))
    ;;
    ;; Fontify function/variable name declarations at the start of the line.
    ;; (Not everyone follows the GNU convention of function name at the start.)
    (list (concat "^" ctoken "[ \t*]+"
		  "\\(" ctoken "[ \t*]+\\)*"
		  "\\(" ctoken "\\)[ \t]*\\((\\)?")
	  2
	  '(if (match-beginning 3)
	       font-lock-function-name-face
	     font-lock-variable-name-face))
    ;;
    ;; Fontify variable names declared with structures, or typedef names.
    '("}[ \t*]*\\(\\sw+\\)[ \t]*[;,[]" 1 font-lock-variable-name-face)
    ;;
    ;; Fontify all builtin keywords (except case, default and goto; see below).
    (concat "\\<\\(" c-keywords "\\)\\>")
    ;;
    ;; Fontify case/goto keywords and targets, and goto tags (incl "default:").
    '("\\<\\(case\\|goto\\)\\>[ \t]*\\([^ \t\n:;]+\\)?"
      (1 font-lock-keyword-face) (2 font-lock-reference-face nil t))
    '("^[ \t]*\\(\\sw+\\)[ \t]*:" 1 font-lock-reference-face)
    )))

 (setq c++-font-lock-keywords-1 c-font-lock-keywords-1)
 (setq c++-font-lock-keywords-2
  (append c++-font-lock-keywords-1
   (list
    ;; We don't just add to the C keywords for subtle differences and speed.
    ;; See the above comments for `c-font-lock-keywords-2'.
    (cons (concat "\\<\\(" c++-type-types "\\)\\>") 'font-lock-type-face)
    (list (concat "\\<\\(" c++-type-types "\\)[ \t*&]+"
		  "\\(" ctoken "[ \t*&]+\\)*"
		  "\\(" ctoken "\\)[ \t]*\\((\\)?")
	  14
	  '(if (match-beginning 15)
	       font-lock-function-name-face
	     font-lock-variable-name-face))
    (list (concat "^" ctoken "[ \t*]+"
		  "\\(" ctoken "[ \t*]+\\)*"
		  "\\(" ctoken "\\)[ \t]*\\((\\)?")
	  2
	  '(if (match-beginning 3)
	       font-lock-function-name-face
	     font-lock-variable-name-face))
    '("}[ \t*]*\\(\\sw+\\)[ \t]*[;,[]" 1 font-lock-variable-name-face)
    (concat "\\<\\(" c++-keywords "\\)\\>")
    '("\\<\\(case\\|goto\\)\\>[ \t]*\\([^ \t\n:;]+\\)?"
      (1 font-lock-keyword-face) (2 font-lock-reference-face nil t))
    '("^[ \t]*\\(\\sw+\\)[ \t]*:[^:]" 1 font-lock-reference-face))))
 )

(setq c++-font-lock-keywords c++-font-lock-keywords-2)

(defun set-txt-fontlock ()
  (interactive)
  (setq font-lock-keywords
        (list
         '("^\\(@calln?\\) \\(.*\\)$"
           (1 font-lock-reference-face)
           (2 font-lock-function-name-face))
         '("^@\\(end\\|begin\\|loop\\).*$" . font-lock-variable-name-face)
         '("^@rem .*$" . font-lock-comment-face)
         ))
  (turn-on-font-lock))

(setq eiffel-font-lock-keywords
      (list 
       '("--.*$" . font-lock-comment-face)
       '("\\(class\\) +\\([a-zA-Z0-9_]+\\)"
         (1 font-lock-keyword-face)
         (2 font-lock-variable-name-face))
       '("\\<\\(feature\\|creation\\|inherit\\|redefine\\)\\>" . font-lock-reference-face)
       '("\\<\\(do\\|begin\\|end\\|while\\|for\\|if\\|then\\|else\\|elseif\\|from\\|until\\|loop\\|invariant\\|require\\|local\\)\\>" . font-lock-keyword-face)
       '(".*\\<is\\>.*$" . font-lock-function-name-face)
         ))

(add-hook 'eiffel-mode-hook
          '(lambda ()
             (setq font-lock-keywords eiffel-font-lock-keywords)))

