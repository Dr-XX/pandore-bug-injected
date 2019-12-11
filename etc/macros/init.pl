#
#  This part is specific to Pandore.
#

%knownarg = ( ACC => '"!acc".$curdim."ef"',
	      VARS => '"!var".$curdim."ef"',
	      LOOP => '"for".$curdim',
	      LOOPP => '"for".$curdim."p"',
	      LOOPIN => '"for".$curdim."in"',
	      LOOPPIN => '"for".$curdim."pin"',
	      LOOPB => '"for".$curdim."b"',
	      LOOPPB => '"for".$curdim."pb"',
	      LOOPINB => '"for".$curdim."inb"',
	      LOOPPINB => '"for".$curdim."pinb"',
	      POINT => '"Point".$curdim',
	      DIM   => '$curdim',
	      VOISS => '(($curdim eq "2d")?4:($curdim eq "3d")?6:2)',
	      VOISL => '(($curdim eq "2d")?8:($curdim eq "3d")?26:2)' );

#
#  Read the defined types from include/panfile.h
#  -> Types are recognized by their name:  Po_[A-Z][a-z0-9]*
#
my @types;
defined($ENV{'PANDOREHOME'}) && open(MAINH,"<$ENV{'PANDOREHOME'}/include/panfile.h") || open(MAINH,"<include/panfile.h");
while(<MAINH>) {
    push @types,$1 if (/Po_([A-Z]\w*).*POBJECT/);
}
close MAINH;

#
#  Read the defined base types from include/ptypes.h
#  Types are recognized by the comment // BASE after the type.
#
my @basetypes;
defined($ENV{'PANDOREHOME'}) && open(PANDH,"<$ENV{'PANDOREHOME'}/include/ptypes.h") || open(PANDH,"<include/ptypes.h");
while(<PANDH>) {
    push @basetypes,$1 if (/typedef.*([A-Z]\w*);.*BASE/);
}
close PANDH;

#
#  Read the defined base types from include/ptypes.h
#  Types are recognized by the comment // BASE IMAGE after the type.
#
my @baseimagetypes;
defined($ENV{'PANDOREHOME'}) && open(PANDH,"<$ENV{'PANDOREHOME'}/include/ptypes.h") || open(PANDH,"<include/ptypes.h");
while(<PANDH>) {
    push @baseimagetypes,$1 if (/typedef.*([A-Z]\w*);.*BASE IMAGE/);
}
close PANDH;

#
#  Return a set of types matching the given regexp
#
my @matchtypes; # @type or @basetype
sub matchtype {
    my $expr = shift;
    my @result = ();
    if ($expr =~ m:/(.*)/:) {
	my $m = $1;
	for (@matchtypes) {
	    push(@result, $_) if ($_ =~ /$m/);
	}
	return @result;
    } else {
# if match is not a regexp return $expr itself
	return ($expr);
    }
}

#
# Idea :  we have a macro 'foo' and a list of set 'S_1, S_2, ..., S_t'
#         we want to generate a call to foo for each tuple 'x_1,...,x_t'
#         where x_i is in S_i.
#
#
#
#         More over we want some argument to be filled according to the
#         last x_i used.  These arguments are listed in %knowarg
#
# e.g.:  ##begin foo(IM1,LOOP,IM2)
#        ...
#        ##end
#        ##forall(foo,Img2du,Img2ds)
# should geneate
#        ##foo(Img2duc,for2d,Img2dsc)
#        ##foo(Img2duc,for2d,Img2dss)
#        ##foo(Img2dus,for2d,Img2dsc)
#        ##foo(Img2dus,for2d,Img2dss)
# where for2d was setup according to the preceding argument
#
my $curdim = undef;		# current dimension (2d, 3d...)
my @realargs = ();		# list of arguments
sub for_all_rec {
    my $arg_nbr = shift;	# The current argument filled with curset
    my $name = shift;		# The macro name
    my $curset = shift;		# current set names
    my @othersets = @_;		# others set names;
#
# basic setup and sanity checks
#
    die ("unknow macro `$name'\n") unless (defined($macros{$name}));
    my @args = @{$macros{$name}->arg_list()};
#
# We stop when we have proceced every arguments of the macro
#
    if ($arg_nbr >= @args) {
	@othersets && die 'There are more arguments ('.join(':',@othersets).") than unknown variables.\n";
	return "##$name(".join(',',@realargs).")\n";
    }
#
# Process a known argument
#
    if ( defined( $knownarg{$args[$arg_nbr]} ) ) {
	$realargs[$arg_nbr] = eval($knownarg{$args[$arg_nbr]});
	return for_all_rec($arg_nbr+1,$name,($curset,@othersets));
    }
#
# Fill an unknown argument with all elements of $curset
#
    die "Not enough argument given to fill unknown variables.\n" unless (defined($curset));
    my @typeset = matchtype($curset);
#    die "Empty set `$curset'\n" if (0 == @typeset);
    my $buf = "";		# buffer for result
    for (@typeset) {
	$realargs[$arg_nbr] = $_;
	$curdim = $1 if /(\dd)/;	# 2d, 3d...
	$buf .= for_all_rec($arg_nbr+1,$name,@othersets);
    }
    return $buf; 
}

# register the builtins

$macros{'forall'} = Macro->builtin('forall', 
				   sub { 
				       @matchtypes = @types;
				       $curdim = undef; 
				       @realargs = ();
				       for_all_rec(0,@_); 
				   });

$macros{'forallbase'} = Macro->builtin('forallbase', 
    sub { 
	@matchtypes = @basetypes;
	$curdim = undef; 
	@realargs = ();
	for_all_rec(0,@_); 
    }); 

$macros{'forallbaseimage'} = Macro->builtin('forallbaseimage', 
    sub { 
	@matchtypes = @baseimagetypes;
	$curdim = undef; 
	@realargs = ();
	for_all_rec(0,@_); 
    }); 

# include a file, with @types en @basetypes and @baseimagetypes filtered according to
# $typefilter and $basetypesfilter and $baseimagetypesfilter

$macros{'include'} = Macro->builtin('include',
    sub {
	$filename = shift || die "filename required";
	$typefilter = shift || '.';
	$basetypesfilter = shift || '.';
	$baseimagetypesfilter = shift || '.';
	my @old_types = @matchtypes = @types;
	@types = matchtype($typefilter);
	my @old_basetypes = @matchtypes = @basetypes;
	@basetypes = matchtype($typefilter);
	my @old_baseimagetypes = @matchtypes = @baseimagetypes;
	@baseimagetypes = matchtype($typefilter);
	open(FILEN,"<$filename") || die "cannot open $filname";
	expandmacros($_) while (<FILEN>);
	close FILEN;
	@types = @old_types;
	@basetypes = @old_basetypes;
	@baseimagetypes = @old_baseimagetypes;
	"";
    });

1;
