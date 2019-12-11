#!perl -w

use Macro;

   %macros = ();  # hash of all macros objects
my $tmp;          # current macro being defined
my $name = undef; # defined when a macro is being defined
my $nested = 0;   # level of nested definition

sub expandmacros {
    $_ = shift || "";

    my @lines = split(/(.*\n)/);

    for (@lines) {
	next if (/^\#\#\;/); ## comments
	if (defined($name)) { 
	    ## if we are defining a macro, look for ##end or
	    ## continue to add text to the current macro.
	    ## As definitions can be nested, we also have to count
	    ## each ##begin/##end pair.
	    if (/^\#\#\s*(begin|append)\b/) {
		$tmp->addtext($_);	    
		++$nested;
	    } elsif (/^\#\#\s*end\b/) {
		if ($nested == 0) {
		    $macros{$tmp->name()} = $tmp;
		    $name = undef;
		} else {
		    --$nested;
		    $tmp->addtext($_);	    
		}
	    } else {
		$tmp->addtext($_);	    
	    }
	} else {
	    ## if we are out of a macro, just look for ##begin
	    ## or macros to expand.
	    if (/^\#\#\s*(begin|append)[^\w!]+(!?\w+)((?:\W+\w+)*)\W*$/) {
		$name = $2;
		my @opts = split(/\W+/,$3);
		shift(@opts);
		if ($1 eq 'begin' || not defined($macros{$name})) {
		    $tmp = Macro->new($name,@opts);
		} else {
		    $tmp = $macros{$name};
		    $tmp->addopts(@opts);
		}
	    } elsif (/^\#\#\s*(.*)$/) {
		## We want to convert a something like 
		## ##foo(bar,quux)
		## into
		## $macros{'foo'}->evaluate('bar',$macros{'quux'}->evaluate)
		## when foo and quux are macros.  Once we have this
		## line, we can get Perl to evaluate it.  
		##
		## To accomplish this, we first split the string at
		## [(),] , then for each splited word defined in
		## %macros we substitute $macros{'word'}->evaluate
		## while other words get quoted.
		##
		## inner macro calls without arguments (such as quux in
		## the previous example) won't be `->evaluate'd if they 
		## takes arguments.

	        my $chaine="".$1; # Fix bug: can't use $1 in split as argument
                                  # -> use a copy.
	        my @tokens = split(/\s*([(),]|"[^\"]*"|'[^\']*')\s*/,$chaine);
		my $i;
		for ($i = 0; $i < @tokens; ++$i) {

		    if (defined($macros{$tokens[$i]}) && 
			($i == 0 || $tokens[$i+1] eq '(' || 
			 scalar(@{$macros{$tokens[$i]}->arg_list()}) == 0)) {
			$tokens[$i] = '$macros{\''.$tokens[$i].'\'}->evaluate';
		    } elsif ($tokens[$i] =~ /^\s*[^(),]/) {
			$tokens[$i] =~ s/^(.*)$/\"\Q$1\E\"/;# quote other words
		    }
		}
		$_ = join(' ',@tokens);
		$_ = eval($_);
		if ($@) {  # did something went wrong ?
		    $! = 1;
		    $@ =~ s/at.*, near/near/;
		    die "$ARGV:$.:\n  $@";
		}
	        expandmacros($_);
	    } else {
		print;
	    }
	}
    }
}

############################################################
# builtin macros
############################################################

sub init_builtins {
    $macros{'dnl'} = Macro->builtin('dnl', 
				    sub {
					$_ = join '',@_; 
					s/\n//gs;
					return $_;
				    });
    $macros{'dlnl'} = Macro->builtin('dlnl', 
				    sub {
					$_ = join '',@_; 
					s/\n$//s;
					return $_;
				    });
}

############################################################
# main
############################################################
init_builtins();
require 'init.pl' || warn "coulnd't read init.pl: $@\n";
while (<>) {
    expandmacros($_);
    close ARGV if (eof ARGV);	# reset $. each file
}
