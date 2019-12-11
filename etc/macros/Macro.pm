package Macro;
use strict;

#  Macro->new(name,arg_list...)
#  Create a new empty macro.
sub new {
    my $class = shift;
    my $self = {};
    bless $self, $class;
    $self->{NAME} = shift; 
    @{ $self->{ARG_LIST} } = @_;
    $self->{BUFFER} = "";
    $self->{BUILTIN} = undef;
    return $self;
}

# Create a builtin macro
# (contains a reference to a Perl function)
sub builtin {
    my $class = shift;
    my $self = {};
    bless $self, $class;
    $self->{NAME} = shift; 
    @{ $self->{ARG_LIST} } = ();
    $self->{BUFFER} = undef;
    $self->{BUILTIN} = shift;
    return $self;
}

sub name {
    my $self = shift;
    if (@_) { $self->{NAME} = shift }
    return $self->{NAME};
}

sub arg_list {
    my $self = shift;
    if (@_) { ${ $self->{ARG_LIST} } = @_ }
    return $self->{ARG_LIST};
}

sub addtext {
    my $self = shift;
    $self->{BUFFER} .= join('',@_);
}

sub addopts {
    my $self = shift;
    push @{ $self->{ARG_LIST} }, @_;
}

#  $obj->evaluate(args...)
#  evaluate the macro, with args as values for each element of ARG_LIST
sub evaluate {
    my $self = shift;
    return &{$self->{BUILTIN}}(@_) if (defined($self->{BUILTIN}));
    
    my $l = scalar(@{$self->{ARG_LIST}});
    my $r = scalar(@_);
    if ( $l != $r ) {
	die "Wrong number of arguments ($r, should be $l) in evaluation of `".$self->name()."'.\n";
    }

    my $outp = $self->{BUFFER};
    for (@{$self->{ARG_LIST}}) {
	my $arg = $_;
	my $value = shift;
	$outp =~ s/\$?\b$arg\b\$?/$value/gs;
    }
    $outp =~ s/\n//gs if ($self->name() =~ /^!/);
    return $outp;
}

1;

