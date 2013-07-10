This is about Radiative Muon Capture Background Study:
===

##Hi
hello

##Try LATEX
%########################################################
\chapter{Backgrounds Estimation} \label{ch:Backgrounds}
%########################################################
% Interaction with svn don't change these by hand (they are automatically updated
%
\svnidlong
{$LastChangedBy: $}
{$LastChangedRevision: $}
{$LastChangedDate: $}
{$HeadURL: $}
%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Background Estimation
%
\section{Background Sources for \muec Conversion}\label{sec:Backgrounds}
%
Potential background sources for the search for \muec conversion are grouped into four categories.
%
\begin{itemize}
\item intrinsic physics backgrounds,
\vspace{-3mm}
\item beam-related prompt backgrounds,
\vspace{-3mm}
\item beam-related decayed backgrounds, and
\vspace{-3mm}
\item other backgrounds including cosmic ray backgrounds.
\end{itemize}
%
The intrinsic physics backgrounds come from muons stopped in the muon stopping target. The beam-related prompt backgrounds are background events originated from protons coming and hitting the proton target between beam pulses. The beam-related delayed backgrounds are backgrounds originating from the main proton beam pulse and coming at a later time within the measurement time window. A list of background events are summarized in Table~\ref{tb:backgrounds}.  

\begin{table}[htb!]
 \caption{A list of potential backgrounds for a search for \muec conversion.}
 \label{tb:backgrounds}
  \begin{center}
   \begin{tabular}{|l|l|}
    \hline \hline
       \multicolumn{2}{|c|}{Intrinsic physics backgrounds} \\
    \hline
    Muon decay in orbit &
    Bound muons decay in a muonic atom \\ \hline
    Radiative muon capture (external)
    & $\mu^- + A \rightarrow \nu_\mu + A'$,
    $A' \rightarrow \gamma + A$, \\
    &followed by $\gamma \rightarrow e^- + e^+$ \\ \hline
    Radiative muon capture (internal)
    & $\mu^- + A \rightarrow \nu_\mu + A'$,
    $A' \rightarrow e^{+} + e^{-} + A$, \\ \hline
    Neutron emission after
    & $\mu^- + A \rightarrow \nu_\mu + A'$,
    $A' \rightarrow n + A$, \\
    after $\mu^{-}$ capture & and neutrons produce $e^-$ \\ \hline
    Charged particle emission
    & $\mu^- + A \rightarrow \nu_\mu + A'$, \\
    after $\mu^{-}$ capture &  $A' \rightarrow p$ (or $d$ or $\alpha$) $+ A$\\
    & followed by $p$ (or $d$ or $\alpha$) produce $e^-$ \\
    \hline \hline
    \multicolumn{2}{|c|}{Beam related prompt backgrounds} \\
    \hline
    Radiative pion capture (external)
    & $\pi^- + A \rightarrow \gamma + A'$ \\
    & followed by  $\gamma \rightarrow e^- + e^+$  \\ \hline
    Radiative pion capture (internal)
    & $\pi^- + A \rightarrow e^{+} + e^{-} + A'$ \\ \hline
    Beam electrons
    & $e^-$ scattering off a muon stopping target \\ \hline
    Muon decay in flight
    & $\mu^-$ decays in flight to produce $e^-$ \\ \hline
    Pion decay in flight
    & $\pi^-$ decays in flight to produce $e^-$ \\ \hline
    Neutron induced backgrounds
    & neutrons hit material to produce $e^-$ \\ 
    \hline \hline
    \multicolumn{2}{|c|}{Beam related delayed backgrounds} \\
    \hline
    Delayed-pion radiative capture
    & $\pi^- + A \rightarrow \gamma + A'$,
    $\gamma \rightarrow e^- + e^+$ \\ \hline
    $\overline{p}$ induced backgrounds
    & $\overline{p}$ hits material to produce $e^-$ \\
    \hline \hline
     \multicolumn{2}{|c|}{Other backgrounds} \\
    \hline
    Cosmic-ray induced backgrounds & \\ \hline
    False tracking & \\ \hline
\end{tabular} 
\end{center}
\end{table}

\section{Intrinsic physics backgrounds} \label{sec:IntrinsicPhysicsBackground}

Negative muons stopped in material are immediately trapped by the Coulomb potential of the nucleus of the material, and fall down to the 1S orbit of a muonic atom. There are two major allowed processes in which a bound $\mu^{-}$ could proceed. They are
%
\begin{itemize}
\item muon decays in orbit (DIO), and
\vspace{-3mm}
\item nuclear muon capture (NMC).
\end{itemize}
%
%%% MW not necessary MW : \noindent These processes are allowed in the Standard Model of particle physics. They would become potential backgrounds in the search for \muec conversion.

\subsection{Muon decays in orbit}\label{sec:diobackground}

\begin{figure}[htb!]
 \begin{center}
 \includegraphics[width=100mm]{figs/DIOspectrum.pdf}
 \end{center}
 \caption{Energy spectrum of electrons from muon decay in orbit in
muonic aluminum.}
 \label{fig:diospectrum}
\end{figure}

Muon DIO is a Michel decay, $\mu^{-} \rightarrow e^{-} \nu_{\mu} \overline{\nu}_{e}$, of the muons that are bound in a muonic atom under a Coulomb potential of the nucleus. Because of the recoil of the nucleus, an electron from the Michel decay can be boosted. The maximum energy of the $e^{-}$ exceeds the end point energy of the ordinary Michel decay of 52.8 MeV and extends to the momentum range of the \muec conversion signal. This is one of the dominant background sources. 

The momentum spectrum of electrons from muon decay in orbit for aluminum can be calculated based on the model described in \cite{Czarnecki11}. Figure~\ref{fig:diospectrum} shows the momentum spectrum of DIO electrons from aluminum. Based on the calculated spectrum, DIO electrons are generated in a Monte Carlo simulation and their tracks are reconstructed with Genfit. 
Fig.~\ref{fig:signaldiomomentum} shows the reconstructed momentum spectrum of DIO electrons with \muec conversion signals.
%Figure~\ref{fig:reconstructed-dio} shows the reconstructed momentum spectra of DIO electrons with \muec conversion signals for different trigger counters.
%
%\begin{figure}[htb!]
% \begin{center}
% (a) \includegraphics[width=60mm]{figs/reconstructed-momentum-noabsorber.pdf}
% (b) \includegraphics[width=60mm]{figs/reconstructed-momentum-5mm.pdf}
% (c) \includegraphics[width=60mm]{figs/reconstructed-momentum-75mm.pdf}
% \end{center}
% \caption{Reconstructed DIO electron momentum spectra with \muec conversion signals for different trigger counters. (a) No trigger counter,  (b) trigger counter of 5 mm thickness, (c) trigger counter of 7.5 mm thickness.}
% \label{fig:reconstructed-dio}
%\end{figure}

This momentum spectrum is then integrated above the momentum threshold of the \muec conversion signal region. Figure~\ref{fig:reconstructed-dio-threshold} shows the integrated fraction of DIO events as a function of momentum threshold, and the fraction of \muec conversion signal events,. The momentum range of the \muec conversion signal region is determined so that the fraction of DIO electrons in the signal region is 0.01 events. From this, as described before, the momentum cut of $103.8~{\rm MeV}/c < P_{e} < 106~{\rm MeV}/c$ is determined.

\begin{figure}[htb!]
 \begin{center}
 \includegraphics[width=100mm,angle=270]{figs/mom_acceptance.pdf}
% \includegraphics[width=100mm]{figs/threshold-dio-5mm.pdf}
 \end{center}
 \caption{Event fractions for DIO electron events and \muec conversion events as a function of lower momentum threshold of the signal region.}
 \label{fig:reconstructed-dio-threshold}
\end{figure}

\subsection{Beam-related prompt backgrounds}

The beam-related prompt backgrounds arise from protons between the beam pulses. These backgrounds are suppressed by the proton beam extinction factor. The proton beam extinction factor, $R_{\mbox{\scriptsize extinction}}$ of $3 \times 10^{-11}$ is used, since $R_{\mbox{\scriptsize extinction}}$ of $3 \times 10^{-11}$ was obtained from our recent experimental measurements at the J-PARC MR.\footnote{The proton extinction factor of $10^{-10}$ is required for the Mu2e experiment at FNAL.} 

\subsection{Radiative pion capture}
%
The radiative pion capture (RPC) background is caused by pions contaminated in the muon beam. Those pions are stopped in the muon stopping target, and are captured by an aluminum nucleus immediately to form an excited state of the daughter nucleus. 

There are two processes, one of which produces directly a $\gamma$ ray of high energy (direct process) and the other produces $\gamma$ rays as an evaporate process of the excited daughter nucleus to their ground state. During these processes, a probability of  $\gamma$ ray emission is as high as 2\%.
Some of such emitted $\gamma$ rays convert into $e^{-}$ and $e^{+}$ in the target or in material outside the target. When the $e^{+}e^{-}$ pair creation occurs in an asymmetric energy distribution and the $e^{-}$ has a high energy, it would mimic \muec conversion signals. This is external conversion of a photon from RPC. And also there is an internal conversion of RPC, which contributes to almost same rate.

The probability of $\gamma$ emission and the energy spectrum of $\gamma$s were extensively studied experimentally and theoretically more than 20 years ago. According to \cite{amaro97}, the probability of $\gamma$ emission has very small 
$Z$ dependence. It is almost 2\% for C, O, and Ca as shown in Table~\ref{tbl:rpc_prob}.
%
\begin{table}[!htb]
 \caption{Experimentally measured branching ratios of
radiative pion capture.
This table was taken from the Table~4 in \cite{amaro97}.}
 \label{tbl:rpc_prob}
  \begin{center}
   \begin{tabular}{|ll|}
    \hline
    $^{12}$C & $1.84\pm 0.08$, $1.92\pm 0.91$, $1.6\pm 0.1$ \\
    $^{16}$O & $2.27\pm 0.24$, $2.24\pm 0.48$ \\
    $^{40}$Ca & $1.82\pm 0.05$ \\
    \hline
\end{tabular} 
\end{center}
\end{table}
%
The energy of $\gamma$ from RPC ranges from 50~MeV to 140~MeV as shown in Fig.~\ref{fig:rpc_spectrum}
%
\begin{figure}[bt!]
 \begin{center}
   \includegraphics[width=0.9\hsize]{figs/amaro_rpc_spectrum.pdf}
%  \includegraphics[width=0.9\hsize]{figs/amaro_rpc_spectrum.pdf}
 \end{center}
 \caption{
Momentum distribution of $\gamma$s from RPC.
This figure was reprinted from \cite{amaro97}.
The histogram is experimental data,
and the lines are from some theoretical models.
}
 \label{fig:rpc_spectrum}
\end{figure}
%
The overall shapes of the spectra are very similar between C, O, and Ca.
Therefore, we implemented the experimentally obtained spectrum for Ca
to our Monte Carlo code for this study.

The number of RPC backgrounds is expressed as
%
\begin{eqnarray}
N_{\mbox{\scriptsize RPC}} & = & N_{\rm proton} \times R_{\mbox{\scriptsize extinction}}
\times R_{\pi-\mbox{\scriptsize stop}/p}
\times \nonumber \\
& & P_{\mbox{\scriptsize RPC}}
\times P_{\gamma-e^-}
\times A_{\mbox{\scriptsize geometry}}
\times A_{\mbox{\scriptsize tracking}} \,, \label{eq:rpc}
\end{eqnarray}
where
$N_{\rm proton}$ is a total number of protons on the pion production target,
$R_{\mbox{\scriptsize extinction}}$ is a proton beam extinction factor,
$R_{\pi-\mbox{\scriptsize stop}/p}$ is a number of $\pi^{-}$s coming to a muon stopping target per proton,
$P_{\mbox{\scriptsize RPC}}$ is a branching ratio of radiative muon capture,
$P_{\gamma-e^-}$ is a probability of conversion of the RPC photon to an electron of 105 MeV/$c$,
$A_{\mbox{\scriptsize geometry}}$ is a detector acceptance of the RPC-originated electrons of 105 MeV/$c$, $A_{\mbox{\scriptsize tracking}}$ is an efficiency of tracking.

Parameters that have to be evaluated specifically for the COMET Phase-I CDC are $R_{\pi-\mbox{\scriptsize stop}/p}$,
$P_{\gamma-e^-}$, $A_{\mbox{\scriptsize geometry}}$, and $A_{\mbox{\scriptsize tracking}}$. In order to estimate those numbers, a Monte Carlo simulation was performed.

From the muon beam simulation in the COMET Phase-I, $R_{\pi-\mbox{\scriptsize stop}/p} = 6.9 \times 10^{-5}$ at the target position, as described in the COMET Phase-I proposalds. It could be compared with that in the COMET Phase-II, of $R_{\pi-\mbox{\scriptsize stop}/p} = 3.5 \times 10^{-7}$. The stopping fraction of pions is about 0.9. The branching ratio of radiative pion capture, 

$P_{\mbox{\scriptsize RPC}}$ was simulated for the COMET Phase-I CDC, based on the photon spectrum in Fig.~\ref{fig:rpc_spectrum}. The simulation was started from the RPC photons generated uniformly inside the muon stopping target (of 17 aluminum disks) and isotropically emitted. A total number of $2 \times 10^7$ photon events were generated. Electrons and positions were created by pair production, and also electrons were generated by Compton scattering, The photon conversion could occur at the muon stopping target (TG), at the CDC inner wall (IW) and outer wall (OW), in the CDC gas (GS), and the CDC trigger counter (TR) and so on.  For the electron events with their momenta above 90 MeV/$c$, tracking was performed. In the COMET Phase-I CDC geometry, relative ratios of electrons between 102 MeV/$c$ to 106 MeV/$c$ occurring at different locations were found to be TG:IW:OW:GS:TR=%20:3:153:2:44. 
19:2:22:1:4. Among them, the electrons from TG and IW cannot be discriminated from the signals, whereas the others can be discriminated.In the signal region of 103.8 MeV/$c$ to 106 MeV/$c$ in momentum, 894 events that occurs at the muon stopping target and CDC inner wall remains. It should be noted that photon conversion mostly occurs inside the muon stopping target. In the end, these would give $P_{\gamma-e^-} \times A_{\mbox{\scriptsize geometry}} \times A_{\mbox{\scriptsize tracking}} = 4.47 \times 10^{-5}$. If we assume that the residual protons are distributed uniformlly between two pulses, which is an interval of 1.751 $\mu$s, given the time window for signal as 700 nsec to 1314 nsec, we have to apply a time window cut to these background events. The cut ratio is thus 0.35. With $N_{P} = 3.8 \times 10^{18}$ protons on target, a total of $2.47 \times 10^{-3}$ background events from the external conversion of radiative pion capture is obtained. The contribution of internal conversion is about the same as that of external conversion. Therefore, $N_{\mbox{\scriptsize RPC}} = 0.00493$ events is estimated with a proton beam extinction factor of $3 \times 10^{-11}$.

\section{Beam-related backgrounds}
%
\subsection{Beam electrons, electrons from muon and pion decays in flight}

Beam electron background is caused by electrons contaminating in the muon beam. The electrons in the muon beam mostly originate decays of $\pi^0$s produced by proton bombardment. Electrons will scatter off the muon-stopping target and fake the signal electrons if the electron momentum is in the signal region.

Similarly, decays of muons in flight during the muon transport system can produce energetic electrons that have sufficient total momentum. For the decay electrons to have $p_{\rm total}>102$~MeV/$c$, the muon momentum must exceed 77~MeV/$c$ ($p_{\mu} > 77$~MeV/$c$). Furthermore, $\pi \rightarrow e + \nu$ decay of pions in flight are also a potential source of background. The $\pi$ momentum must exceed 60~MeV/$c$ to make this background process. It is noted that the branching ratio of $\pi \rightarrow e + \nu$ is about $1.2 \times 10^{-4}$.
%
\begin{eqnarray}
N_{\mbox{\scriptsize e-scat}} & = & N_p \times R_{\mbox{\scriptsize extinction}}
\times R_{\mbox{\scriptsize e-beam}/p}
\times R_{\mbox{\scriptsize e-det}}
\times P_{\mbox{\scriptsize e-signal}}
\label{eq:e-scat}
\end{eqnarray}

Monte Carlo beam simulations based on Geant4 were made to estimate energetic electrons in the muon beam. which includes the beam electrons, electrons from muon and pion decays in flight. After the beam collimator, a number of all the electrons with their momentum greater than 80 MeV/$c$ is $R_{\mbox{\scriptsize e-beam}/p} = 7.24 \times 10^{-5}$ per proton. It should be noted that an electron should have transverse momentum, $P_T$, greater than 70 MeV/$c$ to reach the CDC.

The electrons thus obtained from the Monte Carlo beam simulations are injected into the COMET Phase-I CDC to examine what is a probability for them to reach the CDC. In order to evaluate the probability of large angle scattering we mannually implemented G4UrbanMscModel95 Model in a small program to generate scattered electron profile, and subsequently start a COMET Phase-I CDC Geant4 simulation using this profile. Combining them together, $R_{\mbox{\scriptsize e-det}} \times P_{\mbox{\scriptsize e-signal}} < 5 \times 10^{-7}$ is obtained. Considering that electrons are uniformly distributed between pulses, an extra time window cut of 0.35 as mentioned above should be applied. Therefore, from Eq.(\ref{eq:e-scat}), the estimated background is less than $0.00145$.

\subsection{Background induced by beam neutrons}

Background induced by neutrons in a beam with high kinetic energy coming through the muon beam line is estimated. Those neutrons could pass through the muon beam line by continuously reflecting from its inner sides of the beam duct. In principle the neutrons which can produce electrons of 100~MeV must exceed its kinetic energy of 100~MeV. And by using GEANT3 Monte Carlo simulation, an average transit time of
those neutrons arriving at the muon stopping target is estimated
and it is about 300~nsec, and much less than the time of 700~nsec before the measurement time window opens.
Therefore, it is regarded as a prompt background.

From simulation works, it turns out that most probable process to produce a 100 MeV electron is $\pi^0$ production from energetic neutrons, followed by $\pi^0$ decays with photon conversion. Neutrons with their kinetic energy of more than 180~MeV (about 600 MeV/$c$ in momentum) were produced and transported through the COMET Phase-I beam line to examine their rate and energy distributions. The rate of $R_{\mbox{\scriptsize n}/p}$ of $10^{-5}$ neutrons per proton is obtained at the end of the first 90$^{\circ}$ bend. Then, by using Geant simulation, out of a total number of neutrons of $8 \times 10^{8}$, 57 $\pi^0$s are produced with interaction of neutrons to the muon stopping target made of aluminum, yielding a rate of $R_{\mbox{\scriptsize $\pi^0$/n}}$ of $7 \times 10^{-8}$. Then, by producing $10^{6}$ $\pi^0$s following the above kinetic distributions, about 16 electrons fall into the signal region. From this, the rate of $R_{\mbox{\scriptsize e/$\pi^0$}}$ of $1.6 \times 10^{-6}$ is obtained. The prompt background rate $N_{\mbox{\scriptsize neutron}}$ can be estimated by
%
\begin{eqnarray}
N_{\mbox{\scriptsize neutron}} & = & N_p \times R_{\mbox{\scriptsize extinction}}
\times R_{\mbox{\scriptsize n}/p}
\times R_{\mbox{\scriptsize $\pi^0$/n}}
\times R_{\mbox{\scriptsize e/$\pi^0$}}
\label{eq:nbg}
\end{eqnarray}
%
This gives about $4 \times 10^{-10}$.  The neutron background through $\pi^0$s can be negligible.

\section{Beam Related Delayed Backgrounds}

The beam related prompt background events are also expected to be the same for both the COMET Phase-I and the full COMET.

\section{Cosmic ray induced backgrounds}

The background events induced by cosmic rays are proportional to the total running time. The running time of COMET Phase-I is short, at $10^{6}$ sec, in comparison to that of the full COMET of $2\times 10^{7}$ sec. Therefore, the estimated background events are about a factor of 20 less than that of the full COMET experiment.

\section{Summary of background estimations}

Table~\ref{tab:SummaryBackgrounds} shows a summary of the estimated backgrounds. The total estimated background is about 0.0285 events for a single event sensitivity of $3 \times 10^{-15}$ with a proton extinction factor of $3 \times 10^{-11}$. If the proton extinction factor is increased, the expected background events are further reduced.

\begin{table}[tb!]
	\begin{center}
	\caption{Summary of estimated background events for a single-event sensitivity of $3 \times 10^{-15}$ with a proton extinction factor of $3 \times 10^{-11}$. The numbers with $*$ is directly proportional to the proton extinction factor.} \label{tab:SummaryBackgrounds}
	\vspace{5mm}
		\begin{tabular}{|l|c|} \hline
		Background & estimated events \cr\hline \hline
		Muon decay in orbit & 0.01 \cr \hline
		Radiative muon capture & $<0.001$ \cr \hline
		Neutron emission after muon capture & $<0.001$ \cr \hline
		Charged particle emission after muon capture & $<0.001$ \cr \hline
		Radiative pion capture &  $0.00493^{*}$ \cr \hline
		Beam electrons &  \cr
		Muon decay in flight & $0.00145^{*}$ \cr
		Pion decay in flight &  \cr \hline
		Neutron induced background & $\sim 0^{*}$ \cr \hline
		Delayed radiative pion capture & 0.002 \cr \hline
		Anti-proton induced backgrounds & 0.007 \cr \hline
		Electrons from cosmic ray muons & $<0.0001$ \cr\hline
		Total & 0.0285 \cr\hline
		\end{tabular}
	\end{center}
\end{table}

%
% end
