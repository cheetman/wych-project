package com.tes.project.adapters;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.tes.project.R;
import com.tes.project.databinding.ItemFileBinding;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class RvFilesAdapter extends RecyclerView.Adapter<RvFilesAdapter.MyViewHolder> {

    private Context mContext;
//    private List<String> mList = new ArrayList<>();
    private List<File> mList;

    public RvFilesAdapter(Context mContext , List<File>  mList){
        this.mContext = mContext;
        this.mList = mList;
    }

    public RvFilesAdapter(Context mContext ){
        this.mContext = mContext;
        this.mList  = new ArrayList<>();
    }




    public void setList(List<File> mList){
        this.mList = mList;
        notifyDataSetChanged();
    }

    private OnItemClickListener mOnItemClickListener;
    public void setOnItemClickListener(OnItemClickListener mOnItemClickListener){
        this.mOnItemClickListener = mOnItemClickListener;
    }





    @NonNull
    @Override
    public MyViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {

        //以前写法
        //MyViewHolder holder = new MyViewHolder(LayoutInflater.from(mContext).inflate(R.layout.item_file,parent,false));

        ItemFileBinding binding = ItemFileBinding.inflate(LayoutInflater.from(parent.getContext()), parent, false);
        MyViewHolder holder = new MyViewHolder(binding);


        return holder;
    }

    @Override
    public void onBindViewHolder(@NonNull MyViewHolder holder, int position) {
        holder.binding.tvName.setText(mList.get(position).getName());
        holder.binding.tvNo.setText(String.valueOf(position + 1));
        if(mOnItemClickListener != null){
            holder.itemView.setOnClickListener(view -> {
                int pos = holder.getLayoutPosition();
                mOnItemClickListener.onItemClick(holder.binding.tvName,pos);
            });
            holder.itemView.setOnLongClickListener(view -> {
                int pos = holder.getLayoutPosition();
                mOnItemClickListener.onItemLongClick(holder.binding.tvName,pos);
                return false;
            });


        }
    }

    @Override
    public int getItemCount() {
        return mList.size();
    }

    class MyViewHolder extends  RecyclerView.ViewHolder{

        ItemFileBinding binding;

        public MyViewHolder(@NonNull ItemFileBinding binding) {
            super(binding.getRoot());
            this.binding = binding;
        }
    }


    public interface OnItemClickListener {
        void onItemClick (View  view,  int position) ;
        void onItemLongClick (View view, int position) ;

    }
}
