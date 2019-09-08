(setq exec-path 
      (append 
       (list
	"/usr/local/bin"
	)
       exec-path))

(setq load-path
      (append 
       (list
	(expand-file-name "~")
	(expand-file-name "~/emacs"))
       load-path))

(put 'erase-buffer 'disabled nil)
(put 'downcase-region 'disabled nil)
(put 'upcase-region 'disabled nil)
(put 'eval-expression 'disabled nil)

(if (eq window-system 'x) 
    (progn (set-face-background 'region "rgb:50/00/00")
	   (transient-mark-mode 1)
	   (standard-display-european 1)
	   (setq Man-notify 'newframe)
	   (menu-bar-mode 1)
	   (load "font-lock-start") ))

(require 'jka-compr)

(setq jka-compr-use-shell nil)

(setq c-mode-hook 'c++-mode)

(setq next-line-add-newlines nil)
(setq auto-save-list-file-prefix nil)
(set-face-background 'highlight "forestgreen")
(set-face-background 'region "rgb:90/10/10")

; tta mud ftp
(defun tta () "Edit on tta"
  (interactive)
  (require 'ange-ftp)
  (ange-ftp-parse-netrc)
  (find-file "/tta.sieni@tta.artis.uni-oldenburg.de:/"))

; key definitions + related functions
(defun easy-scroll-left (&optional arg)
  (interactive "P")
  (scroll-left (or arg 4)))

(defun easy-scroll-right (&optional arg)
  (interactive "P")
  (scroll-right (or arg 4)))

(defun easy-scroll-up (&optional arg)
  (interactive "P")
  (scroll-up (or arg 4)))

(defun easy-scroll-down (&optional arg)
  (interactive "P")
  (scroll-down (or arg 4)))

(defun my-fill-column (&optional arg)
  (interactive "P")
  (setq fill-column 79))

(define-key global-map '[S-up] 'easy-scroll-down)
(define-key global-map '[S-left] 'easy-scroll-right)
(define-key global-map '[S-right] 'easy-scroll-left)
(define-key global-map '[S-down] 'easy-scroll-up)

(global-set-key '[C-delete] 'undo)
(global-set-key '[menu] 'eval-current-buffer)
(global-set-key '[f4] 'speedbar-get-focus)
(global-set-key '[f6] 'other-window)
(global-set-key '[f7] 'new-frame)
(global-set-key '[f8] 'delete-frame)
(global-set-key '[?\M-b] 'bury-buffer)
(global-set-key '[kp-f1] 'justify-current-line)
(global-set-key '[kp-f2] 'my-fill-column)

(add-hook 'term-setup-hook '(lambda () (message "Welcome!")))

(defun my-c-offsets () ""
  (setq c-basic-offset 4)
  (setq c-offsets-alist
	'((string                . -1000)
	  (c                     . c-lineup-C-comments)
	  (defun-open            . 0)
	  (defun-close           . 0)
	  (defun-block-intro     . +)
	  (class-open            . 0)
	  (class-close           . 0)
	  (inline-open           . +)
	  (inline-close          . 0)
	  (ansi-funcdecl-cont    . -)
	  (knr-argdecl-intro     . +)
	  (knr-argdecl           . 0)
	  (topmost-intro         . 0)
	  (topmost-intro-cont    . 0)
	  (member-init-intro     . +)
	  (member-init-cont      . 0)
	  (inher-intro           . +)
	  (inher-cont            . c-lineup-multi-inher)
	  (block-open            . 0)
	  (block-close           . 0)
	  (brace-list-open       . 0)
	  (brace-list-close      . 0)
	  (brace-list-intro      . 0)
	  (brace-list-entry      . 0)
	  (statement             . c-lineup-runin-statements)
	  (statement-cont        . c-lineup-math)
	  (statement-block-intro . +)
	  (statement-case-intro  . +)
	  (statement-case-open   . 0)
	  (substatement          . +)
	  (substatement-open     . 0)
	  (case-label            . 2)
	  (access-label          . 0)
	  (label                 . 2)
	  (do-while-closure      . 0)
	  (else-clause           . 0)
	  (comment-intro         . c-lineup-comment)
	  (arglist-intro         . +)
	  (arglist-cont          . 0)
	  (arglist-cont-nonempty . c-lineup-arglist)
	  (arglist-close         . +)
	  (stream-op             . c-lineup-streamop)
	  (inclass               . +)
	  (cpp-macro             . -1000)
	  (friend                . 0)
	  (objc-method-intro     . -1000)
	  (objc-method-args-cont . c-lineup-ObjC-method-args)
	  (objc-method-call-cont . c-lineup-ObjC-method-call)
	  ))
  )
  
(add-hook 'java-mode-hook
	  '(lambda ()
	     (local-set-key [return] 'reindent-then-newline-and-indent)
	     (my-c-offsets)
	     ))

(add-hook 'c++-mode-hook 
	  '(lambda ()
	     (local-set-key [return] 'reindent-then-newline-and-indent)
	     (turn-on-font-lock)
	     (my-c-offsets)
	     ))

(line-number-mode t)

(defun emud-enter-docheader (arg)
  "Inserts a documentation header for lpc functions."
  (interactive "P")
  (insert "/*\n * Function name: \n * Description:   \n")
  (insert " * Arguments:     \n * Returns:       \n")
  (insert " */\n")
  )

(defun emud-debug (arg)
  "Insert debug function"
  (interactive "P")
  (insert "debug(\"manwe\", );")
  (reindent-then-newline-and-indent)
  )

(global-set-key '[f9] 'emud-enter-docheader)
(global-set-key '[f10] 'emud-debug)
