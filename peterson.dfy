// There are six possible PCs for each thread, each corresponding to
// the beginning of one atomic step.
datatype PC = SetFlag | SetVictim | CheckFlag | CheckVictim | Crit | ResetFlag

method Peterson(n:int, vic:int, starting_thread:int)
decreases *
requires 2 <= n < 10
requires 0 <= vic < 2
requires 0 <= starting_thread < n
{
  // Thread ids will be ints 0 or 1.
  // Below, we will use 1-t to get the "other" tid from a tid t.

  // We store some variables in arrays so that we can write the code
  // for both threads at once, indexed by the thread id.
  
  var flag := new bool[n];
  var i := 0;
  while i < n
  {
    flag[i] := false;
    i := i + 1;
  }


  // The algorithm is correct regardless of the initial value of
  // victim, so we initialize in nondeterministically.
  var victim := vic;
  //var victim;
  //if * {
  //  victim := 0;
  //} else {
  //  victim := 1;
  //}

  var pc := new PC[n];
  var j := 0;
  while j < n
  invariant forall j' :: 0 <= j' < n && j' < j ==> pc[j'] == SetFlag
  {
    pc[j] := SetFlag;
    j := j + 1;
  }
  
  while true
    // Invariant 1: whenever t's flag is false, its PC is at SetFlag.
    //
    // Comment this out to see that it is required by Invariant 2.
    invariant forall t :: 0 <= t < n && !flag[t] ==> pc[t] == SetFlag

    // Invariant 2: If t holds the lock and 1-t is trying to get it,
    //              then victim == 1-t. (victim != t works too!).
    //
    // Comment this out to see that it is required by the mutual
    // exclusion invariant below!
    //
    // Writing the invariant like this, using t' to represent the
    // other thread, avoids a trigger matching loop.
    invariant forall t, t' :: 0 <= t < n  && 0 <= t' < n && t != t' ==> 
        (pc[t] == Crit || pc[t] == ResetFlag) ==> // if t holds the lock
        (pc[t'] == CheckVictim || pc[t'] == CheckFlag || // and t' is trying to get it
         pc[t'] == Crit || pc[t'] == ResetFlag) ==>
        victim != t // then victim != t
  
     // This is mutual exclusion: It is never the case that both
     // threads are in the critical section.
     //
     // This is directly implied by Invariant 2, since if both PCs are
     // Crit, then the victim cannot be equal to either thread.
     //
     // Dafny is clever enough to infer an additional loop invariant,
     // namely 0 <= victim < 2, which is required to complete the proof.
     // invariant !(pc[0] == Crit && pc[1] == Crit)
    invariant forall t, t' :: ( (0 <= t < n && 0 <= t' < n && pc[t] == pc[t'] && pc[t] == Crit) ==> i == j )
    decreases *
  {
    // Each iteration of the loop simulates one atomic step of execution.

    // First, nondeterministically select the thread that gets to step.
    var t := starting_thread;
    // Now, branch on the selected thread's current PC and execute the
    // corresponding atomic step.
    //
    // Note that each step only reads or writes one shared variable.
    match pc[t] {
      case SetFlag     => flag[t] := true; pc[t] := SetVictim;
      case SetVictim   => victim := t; pc[t] := CheckFlag;
      case CheckFlag   => pc[t] := if (exists t' :: 0 <= t' < n && t' != t && flag[t']) then CheckVictim else Crit;
      case CheckVictim => pc[t] := if victim == t then CheckFlag else Crit;
      case Crit        => pc[t] := ResetFlag;
      case ResetFlag   => flag[t] := false; pc[t] := SetFlag;
    }
  }
}

