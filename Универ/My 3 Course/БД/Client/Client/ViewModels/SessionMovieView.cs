namespace CinemaClientService.ViewModels
{
    public class SessionMovieView
    {
        public int SessionId { get; set; }
        public int MovieId { get; set; }
        
        public string SessionInfo { get; set; } = string.Empty;
        public string MovieInfo { get; set; } = string.Empty;
    }
}