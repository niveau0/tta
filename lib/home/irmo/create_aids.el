; LAST EDIT: Wed Nov  9 15:50:40 1994 by 


(defun create-make-option-list ()
  (let ((i 0)
	name num)
    (insert " ({\n")
    (while
	(re-search-forward
	 "^#define TYPE_\\([A-Z_]+\\)[ \t]*\\([0-9]+\\)$"
	 nil t)
      (setq name (buffer-substring (match-beginning 1)
				   (match-end 1))
	    num (buffer-substring (match-beginning 2)
				  (match-end 2)))
      (replace-match
       (format "(old&TYPE_%s?\"*\":\" \") + \"%s%s%d\\\\n\" +"
	       name (downcase name)
	       (make-string (- 16 (length name)
			       (if (< (setq i (1+ i)) 10)
				   1 2))
			    ?\ )
	       i)))
    (insert (format " })\n\"1..%d to add, -1..-%d to remove or q to quit > \""
		    i i))))
			      
(defun create-make-two-powers (n)
  (let ((i 0)
	(n (or n 31)))
    (while (<= (setq i (1+ i)) n)
      (insert (format "%d\n" (expt 2 i))))))

(defun create-insert-special-rectangle (start end func)
  "Insert a rectangle with each line being calculated by FUNC, which is therefore called with one argument: the line number.
Thus, `identity' for example gives a rectangle of numbers."
  (let (rec
	(i 0)
	(len (count-lines start end)))
    (while (<= (setq i (1+ i)) len)
      (setq rec (cons (format "%s" (eval (list func i))) rec)))
    (goto-char start)
    (insert-rectangle (nreverse rec))))

(defun create-insert-numbered-rectangle (start end &optional begin)
  (interactive "*r\nP")
  (if begin
      (create-insert-special-rectangle start end (lambda (n)
						   (+ n begin)))
    (create-insert-special-rectangle start end 'identity)))

(defun create-insert-powered-rectangle (start end)
  (interactive "*r")
  (create-insert-special-rectangle start end
				   (lambda (n) (expt 2 n))))
