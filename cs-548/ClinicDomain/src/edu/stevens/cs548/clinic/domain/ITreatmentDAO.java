package edu.stevens.cs548.clinic.domain;

public interface ITreatmentDAO {
    
    public static class TreatmentException extends Exception {

        private static final long serialVersionUID = 1L;
        public TreatmentException(String msg) {
            super(msg);
        }
            
    }
    
    public Treatment getTreatmentByDbId(long id) throws TreatmentException;
     
    public void addTreatment(Treatment treatment);
    
    public void deleteTreatment(Treatment treatment);
    
}
